#include <cstdint>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "service/OrderService.h"
#include "repository/IOrderRepository.h"
#include "repository/ICartRepository.h"
#include "repository/IProductRepository.h"
#include "exception/AppException.h"

using namespace dhivagar::dhivagarmart;
using ::testing::_;
using ::testing::Return;

class MockOrderRepository : public repository::IOrderRepository {
public:
    MOCK_METHOD(int64_t, CreateOrderInTransaction, 
                (pqxx::work& tx, int64_t buyer_id, int64_t total_amount_cents, model::OrderStatus status,
                 const std::string& payment_method, const std::string& payment_status,
                 const std::string& delivery_address, const std::string& phone,
                 const std::string& full_name, const std::string& request_id), (override));
    MOCK_METHOD(void, CreateOrderItemInTransaction, 
                (pqxx::work& tx, int64_t order_id, int64_t product_id, int32_t quantity, int64_t unit_price_cents, const std::string& request_id), (override));
    MOCK_METHOD(std::optional<model::Order>, FindById, (int64_t id, const std::string& request_id), (override));
    MOCK_METHOD(std::vector<model::Order>, FindByBuyerId, (int64_t buyer_id, const std::string& request_id), (override));
    MOCK_METHOD(std::vector<model::Order>, FindBySellerId, (int64_t seller_id, const std::string& request_id), (override));
    MOCK_METHOD(std::vector<model::Order>, FindAll, (const std::string& request_id), (override));
    MOCK_METHOD(bool, UpdateStatus, (int64_t order_id, model::OrderStatus status, const std::string& request_id), (override));
    MOCK_METHOD(bool, HasBuyerCompletedPurchaseOfProduct, (int64_t buyer_id, int64_t product_id, const std::string& request_id), (override));
};

class MockCartRepoForOrder : public repository::ICartRepository {
public:
    MOCK_METHOD(std::vector<model::CartItem>, GetCartItems, (int64_t user_id, const std::string& request_id), (override));
    MOCK_METHOD(void, AddOrIncrementItem, (int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id), (override));
    MOCK_METHOD(bool, UpdateQuantity, (int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id), (override));
    MOCK_METHOD(bool, RemoveItem, (int64_t user_id, int64_t product_id, const std::string& request_id), (override));
    MOCK_METHOD(void, ClearCart, (int64_t user_id, const std::string& request_id), (override));
    MOCK_METHOD(void, ClearCartInTransaction, (pqxx::work& tx, int64_t user_id, const std::string& request_id), (override));
};

class MockProductRepoForOrder : public repository::IProductRepository {
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
    MOCK_METHOD(bool, RestoreStockInTransaction, (pqxx::work& tx, int64_t product_id, int32_t quantity, const std::string& request_id), (override));
};

TEST(OrderServiceTest, CheckoutEmptyCartThrowsValidationException) {
    auto order_repo = std::make_shared<MockOrderRepository>();
    auto cart_repo = std::make_shared<MockCartRepoForOrder>();
    auto product_repo = std::make_shared<MockProductRepoForOrder>();
    service::OrderService order_service(order_repo, cart_repo, product_repo);

    EXPECT_CALL(*cart_repo, GetCartItems(3, _))
        .WillOnce(Return(std::vector<model::CartItem>{}));

    dto::CreateOrderRequestDto req{
        .shipping_address = "123 Grocery Lane",
        .payment_method = "MOCK_PAYMENT"
    };

    EXPECT_THROW(order_service.Checkout(3, req), exception::ValidationException);
}

TEST(OrderServiceTest, CheckoutInsufficientStockThrowsValidationException) {
    auto order_repo = std::make_shared<MockOrderRepository>();
    auto cart_repo = std::make_shared<MockCartRepoForOrder>();
    auto product_repo = std::make_shared<MockProductRepoForOrder>();
    service::OrderService order_service(order_repo, cart_repo, product_repo);

    std::vector<model::CartItem> items;
    items.push_back(model::CartItem{
        .id = 1,
        .user_id = 3,
        .product_id = 1,
        .product_name = "Honeycrisp Apples",
        .available_stock = 1,
        .quantity = 5 // exceeds stock!
    });

    EXPECT_CALL(*cart_repo, GetCartItems(3, _))
        .WillOnce(Return(items));

    dto::CreateOrderRequestDto req{
        .shipping_address = "123 Main St",
        .payment_method = "MOCK_PAYMENT"
    };

    EXPECT_THROW(order_service.Checkout(3, req), exception::ValidationException);
}
