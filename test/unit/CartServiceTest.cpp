#include <cstdint>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "service/CartService.h"
#include "repository/ICartRepository.h"
#include "repository/IProductRepository.h"
#include "exception/AppException.h"

using namespace dhivagar::dhivagarmart;
using ::testing::_;
using ::testing::Return;

class MockCartRepository : public repository::ICartRepository {
public:
    MOCK_METHOD(std::vector<model::CartItem>, GetCartItems, (int64_t user_id, const std::string& request_id), (override));
    MOCK_METHOD(void, AddOrIncrementItem, (int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id), (override));
    MOCK_METHOD(bool, UpdateQuantity, (int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id), (override));
    MOCK_METHOD(bool, RemoveItem, (int64_t user_id, int64_t product_id, const std::string& request_id), (override));
    MOCK_METHOD(void, ClearCart, (int64_t user_id, const std::string& request_id), (override));
    MOCK_METHOD(void, ClearCartInTransaction, (pqxx::work& tx, int64_t user_id, const std::string& request_id), (override));
};

class MockProductRepoForCart : public repository::IProductRepository {
public:
    MOCK_METHOD(model::Product, Create, (const model::Product& product, const std::string& request_id), (override));
    MOCK_METHOD(std::optional<model::Product>, FindById, (int64_t id, const std::string& request_id), (override));
    MOCK_METHOD(std::vector<model::Product>, FindAll, 
                (const std::optional<std::string>& category, const std::optional<std::string>& search, const std::string& request_id), (override));
    MOCK_METHOD(std::vector<model::Product>, FindBySellerId, (int64_t seller_id, const std::string& request_id), (override));
    MOCK_METHOD(model::Product, Update, (const model::Product& product, const std::string& request_id), (override));
    MOCK_METHOD(bool, Delete, (int64_t id, int64_t seller_id, const std::string& request_id), (override));
    MOCK_METHOD(bool, AdminDelete, (int64_t id, const std::string& request_id), (override));
    MOCK_METHOD(bool, ReduceStockInTransaction, (pqxx::work& tx, int64_t product_id, int32_t quantity, const std::string& request_id), (override));
};

TEST(CartServiceTest, AddItemInsufficientStockThrowsValidationException) {
    auto cart_repo = std::make_shared<MockCartRepository>();
    auto product_repo = std::make_shared<MockProductRepoForCart>();
    service::CartService cart_service(cart_repo, product_repo);

    model::Product low_stock_product{
        .id = 5,
        .seller_id = 2,
        .name = "Limited Edition Item",
        .price = model::Money::FromCents(1000),
        .stock_qty = 2
    };

    EXPECT_CALL(*product_repo, FindById(5, _))
        .WillOnce(Return(low_stock_product));

    dto::AddCartItemRequestDto req{
        .product_id = 5,
        .quantity = 10 // exceeds 2
    };

    EXPECT_THROW(cart_service.AddItem(1, req), exception::ValidationException);
}

TEST(CartServiceTest, RunningTotalCalculation) {
    auto cart_repo = std::make_shared<MockCartRepository>();
    auto product_repo = std::make_shared<MockProductRepoForCart>();
    service::CartService cart_service(cart_repo, product_repo);

    std::vector<model::CartItem> items;
    model::CartItem item1{
        .id = 1,
        .user_id = 3,
        .product_id = 1,
        .product_name = "Apples",
        .product_price = model::Money::FromCents(399),
        .available_stock = 20,
        .quantity = 2 // Subtotal: 798
    };
    model::CartItem item2{
        .id = 2,
        .user_id = 3,
        .product_id = 2,
        .product_name = "Milk",
        .product_price = model::Money::FromCents(449),
        .available_stock = 15,
        .quantity = 1 // Subtotal: 449
    };
    items.push_back(item1);
    items.push_back(item2);

    EXPECT_CALL(*cart_repo, GetCartItems(3, _))
        .WillOnce(Return(items));

    auto cart = cart_service.GetCart(3);
    EXPECT_EQ(cart.total_items, 3);
    EXPECT_EQ(cart.total_cents, 1247); // 798 + 449 = 1247 ($12.47)
    EXPECT_EQ(cart.total_formatted, "$12.47");
}
