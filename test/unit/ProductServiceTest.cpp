#include <cstdint>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "service/ProductService.h"
#include "repository/IProductRepository.h"
#include "exception/AppException.h"

using namespace dhivagar::dhivagarmart;
using ::testing::_;
using ::testing::Return;

class MockProductRepository : public repository::IProductRepository {
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

TEST(ProductServiceTest, GetProductByIdSuccess) {
    auto mock_repo = std::make_shared<MockProductRepository>();
    service::ProductService product_service(mock_repo);

    model::Product p{
        .id = 1,
        .seller_id = 2,
        .name = "Fresh Organic Apples",
        .description = "Crisp apples",
        .price = model::Money::FromCents(399),
        .stock_qty = 50,
        .category = "Fruits",
        .image_url = "",
        .created_at = "2026-09-24"
    };

    EXPECT_CALL(*mock_repo, FindById(1, _))
        .WillOnce(Return(p));

    auto res = product_service.GetProductById(1);
    EXPECT_EQ(res.id, 1);
    EXPECT_EQ(res.name, "Fresh Organic Apples");
    EXPECT_EQ(res.price_cents, 399);
    EXPECT_EQ(res.price_formatted, "₹3.99");
}

TEST(ProductServiceTest, GetProductByIdNotFoundThrows) {
    auto mock_repo = std::make_shared<MockProductRepository>();
    service::ProductService product_service(mock_repo);

    EXPECT_CALL(*mock_repo, FindById(999, _))
        .WillOnce(Return(std::nullopt));

    EXPECT_THROW(product_service.GetProductById(999), exception::NotFoundException);
}
