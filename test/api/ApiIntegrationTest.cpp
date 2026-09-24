#include <gtest/gtest.h>
#include "service/AuthService.h"
#include "service/ProductService.h"
#include "service/ChatService.h"
#include "repository/UserRepository.h"
#include "repository/ProductRepository.h"
#include "plugin/DatabasePlugin.h"

using namespace dhivagar::dhivagarmart;

class ApiIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        plugin::DatabasePlugin::InitAndStart();
    }
};

TEST_F(ApiIntegrationTest, SeededAdminCanLogin) {
    auto user_repo = std::make_shared<repository::UserRepository>();
    service::AuthService auth_service(user_repo);

    dto::LoginRequestDto req{
        .email = "admin@dhivagarmart.com",
        .password = "Admin@123"
    };

    auto res = auth_service.Login(req);
    EXPECT_EQ(res.role, "ADMIN");
    EXPECT_EQ(res.email, "admin@dhivagarmart.com");
}

TEST_F(ApiIntegrationTest, SeededSellerCanLogin) {
    auto user_repo = std::make_shared<repository::UserRepository>();
    service::AuthService auth_service(user_repo);

    dto::LoginRequestDto req{
        .email = "seller@dhivagarmart.com",
        .password = "Seller@123"
    };

    auto res = auth_service.Login(req);
    EXPECT_EQ(res.role, "SELLER");
    EXPECT_EQ(res.email, "seller@dhivagarmart.com");
}

TEST_F(ApiIntegrationTest, SeededBuyerCanLogin) {
    auto user_repo = std::make_shared<repository::UserRepository>();
    service::AuthService auth_service(user_repo);

    dto::LoginRequestDto req{
        .email = "buyer@dhivagarmart.com",
        .password = "Buyer@123"
    };

    auto res = auth_service.Login(req);
    EXPECT_EQ(res.role, "BUYER");
    EXPECT_EQ(res.email, "buyer@dhivagarmart.com");
}

TEST_F(ApiIntegrationTest, ProductCatalogRetrieval) {
    auto prod_repo = std::make_shared<repository::ProductRepository>();
    service::ProductService prod_service(prod_repo);

    auto list = prod_service.GetProducts(std::nullopt, std::nullopt);
    EXPECT_GE(list.size(), 10u);

    // Verify first item contains formatted money
    EXPECT_FALSE(list[0].price_formatted.empty());
    EXPECT_GT(list[0].price_cents, 0);
}

TEST_F(ApiIntegrationTest, ChatbotFaqRepliesAndCache) {
    service::ChatService chat_service;

    std::string session_id = "test-session-123";
    auto resp1 = chat_service.Chat(session_id, "What fresh fruits do you sell?");
    EXPECT_FALSE(resp1.reply.empty());
    EXPECT_FALSE(resp1.cached);

    // Second identical call must be served from cache
    auto resp2 = chat_service.Chat(session_id, "What fresh fruits do you sell?");
    EXPECT_EQ(resp1.reply, resp2.reply);
    EXPECT_TRUE(resp2.cached);
}
