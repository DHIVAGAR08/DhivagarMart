#include <gtest/gtest.h>
#include "service/AuthService.h"
#include "service/ProductService.h"
#include "service/ChatService.h"
#include "service/CartService.h"
#include "service/OrderService.h"
#include "service/AdminService.h"
#include "service/SellerService.h"
#include "repository/UserRepository.h"
#include "repository/ProductRepository.h"
#include "repository/CartRepository.h"
#include "repository/OrderRepository.h"
#include "exception/AppException.h"
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
    EXPECT_GE(list.size(), 6u);

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

TEST_F(ApiIntegrationTest, CartLifecycleAndRunningTotalCalculation) {
    auto cart_repo = std::make_shared<repository::CartRepository>();
    auto prod_repo = std::make_shared<repository::ProductRepository>();
    service::CartService cart_service(cart_repo, prod_repo);

    int64_t buyer_id = 3; // Seeded John Buyer
    cart_service.ClearCart(buyer_id);

    // 1. Initial cart is empty
    auto cart0 = cart_service.GetCart(buyer_id);
    EXPECT_EQ(cart0.total_items, 0);
    EXPECT_EQ(cart0.total_cents, 0);

    // 2. Add product 1 with quantity 2
    dto::AddCartItemRequestDto add_dto{.product_id = 1, .quantity = 2};
    auto cart1 = cart_service.AddItem(buyer_id, add_dto);
    EXPECT_EQ(cart1.total_items, 2);
    ASSERT_EQ(cart1.items.size(), 1u);
    EXPECT_EQ(cart1.items[0].product_id, 1);
    EXPECT_EQ(cart1.items[0].quantity, 2);
    EXPECT_GT(cart1.total_cents, 0);

    // 3. Update quantity to 4 and verify correct subtotal calculation
    dto::UpdateCartItemRequestDto update_dto{.quantity = 4};
    auto cart2 = cart_service.UpdateQuantity(buyer_id, 1, update_dto);
    EXPECT_EQ(cart2.total_items, 4);
    EXPECT_EQ(cart2.items[0].quantity, 4);
    EXPECT_EQ(cart2.total_cents, cart1.items[0].unit_price_cents * 4);

    // 4. Remove item and confirm cart is cleared
    auto cart3 = cart_service.RemoveItem(buyer_id, 1);
    EXPECT_EQ(cart3.total_items, 0);
    EXPECT_TRUE(cart3.items.empty());
}

TEST_F(ApiIntegrationTest, TransactionalCheckoutAndCustomerOrderFlow) {
    auto order_repo = std::make_shared<repository::OrderRepository>();
    auto cart_repo = std::make_shared<repository::CartRepository>();
    auto prod_repo = std::make_shared<repository::ProductRepository>();
    service::CartService cart_service(cart_repo, prod_repo);
    service::OrderService order_service(order_repo, cart_repo, prod_repo);

    int64_t buyer_id = 3;
    cart_service.ClearCart(buyer_id);

    // Add product 1 with quantity 1
    cart_service.AddItem(buyer_id, dto::AddCartItemRequestDto{.product_id = 1, .quantity = 1});

    dto::CreateOrderRequestDto checkout_dto;
    checkout_dto.shipping_address = "42 Technology Park";
    checkout_dto.full_name = "John Buyer";
    checkout_dto.phone = "+91 9876543210";
    checkout_dto.city = "Chennai";
    checkout_dto.state = "Tamil Nadu";
    checkout_dto.pincode = "600032";
    checkout_dto.payment_method = "CASH_ON_DELIVERY";

    auto created_order = order_service.Checkout(buyer_id, checkout_dto);
    EXPECT_GT(created_order.id, 0);
    EXPECT_EQ(created_order.buyer_id, buyer_id);
    EXPECT_EQ(created_order.status, "CONFIRMED");
    EXPECT_EQ(created_order.payment_method, "CASH_ON_DELIVERY");
    EXPECT_EQ(created_order.payment_status, "PENDING");
    EXPECT_FALSE(created_order.delivery_address.empty());
    ASSERT_FALSE(created_order.items.empty());
    EXPECT_EQ(created_order.items[0].product_id, 1);

    // Cart is atomically cleared upon checkout
    auto cart_after = cart_service.GetCart(buyer_id);
    EXPECT_EQ(cart_after.total_items, 0);

    // My Orders retrieval returns customer's placed orders
    auto buyer_orders = order_service.GetBuyerOrders(buyer_id);
    EXPECT_FALSE(buyer_orders.empty());
    bool found = false;
    for (const auto& ord : buyer_orders) {
        if (ord.id == created_order.id) {
            found = true;
            EXPECT_EQ(ord.status, "CONFIRMED");
            break;
        }
    }
    EXPECT_TRUE(found);

    // Restore stock and cancel order to keep DB pristine
    order_service.CancelOrder(created_order.id, buyer_id, false);
}

TEST_F(ApiIntegrationTest, AdminAndSellerOrderModerationAndStatusWorkflow) {
    auto user_repo = std::make_shared<repository::UserRepository>();
    auto prod_repo = std::make_shared<repository::ProductRepository>();
    auto order_repo = std::make_shared<repository::OrderRepository>();
    auto cart_repo = std::make_shared<repository::CartRepository>();

    service::AdminService admin_service(user_repo, prod_repo, order_repo);
    service::SellerService seller_service(prod_repo, order_repo);
    service::OrderService order_service(order_repo, cart_repo, prod_repo);
    service::CartService cart_service(cart_repo, prod_repo);

    int64_t buyer_id = 3;
    cart_service.ClearCart(buyer_id);
    cart_service.AddItem(buyer_id, dto::AddCartItemRequestDto{.product_id = 1, .quantity = 1});

    dto::CreateOrderRequestDto checkout_dto;
    checkout_dto.shipping_address = "100 GST Road";
    checkout_dto.full_name = "John Buyer";
    checkout_dto.phone = "+91 9876543210";
    checkout_dto.city = "Chennai";
    checkout_dto.state = "Tamil Nadu";
    checkout_dto.pincode = "600032";
    checkout_dto.payment_method = "UPI";

    auto order = order_service.Checkout(buyer_id, checkout_dto);
    int64_t order_id = order.id;

    // Admin dashboard features
    auto all_orders = admin_service.GetAllOrders();
    EXPECT_FALSE(all_orders.empty());
    auto admin_stats = admin_service.GetStatistics();
    EXPECT_GT(admin_stats.total_users, 0);
    EXPECT_GT(admin_stats.total_orders, 0);

    // Seller dashboard features
    int64_t seller_id = 2; // Seeded Seller
    auto seller_stats = seller_service.GetSellerStats(seller_id);
    EXPECT_GT(seller_stats.total_products, 0);
    auto seller_orders = seller_service.GetSellerOrders(seller_id);
    EXPECT_FALSE(seller_orders.empty());

    // Seller updates status to SHIPPED
    auto shipped_order = seller_service.UpdateOrderStatus(seller_id, order_id, "SHIPPED");
    EXPECT_EQ(shipped_order.status, "SHIPPED");

    // Admin updates status to DELIVERED
    auto delivered_order = admin_service.UpdateOrderStatus(order_id, "DELIVERED");
    EXPECT_EQ(delivered_order.status, "DELIVERED");

    // Invalid status string validation
    EXPECT_THROW(admin_service.UpdateOrderStatus(order_id, "INVALID_STATUS"), exception::ValidationException);

    // Restore the 1 unit of stock used for this test
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);
    prod_repo->RestoreStockInTransaction(tx, 1, 1);
    tx.commit();
}

TEST_F(ApiIntegrationTest, BuyerOrderCancellationRestoresInventory) {
    auto order_repo = std::make_shared<repository::OrderRepository>();
    auto cart_repo = std::make_shared<repository::CartRepository>();
    auto prod_repo = std::make_shared<repository::ProductRepository>();
    service::CartService cart_service(cart_repo, prod_repo);
    service::OrderService order_service(order_repo, cart_repo, prod_repo);

    int64_t buyer_id = 3;
    cart_service.ClearCart(buyer_id);

    auto prod_before = prod_repo->FindById(1);
    ASSERT_TRUE(prod_before.has_value());
    int32_t initial_stock = prod_before->stock_qty;

    // Place order for 2 units
    cart_service.AddItem(buyer_id, dto::AddCartItemRequestDto{.product_id = 1, .quantity = 2});
    dto::CreateOrderRequestDto checkout_dto;
    checkout_dto.shipping_address = "77 Mount Road";
    checkout_dto.full_name = "John Buyer";
    checkout_dto.phone = "+91 9999988888";
    checkout_dto.city = "Chennai";
    checkout_dto.state = "Tamil Nadu";
    checkout_dto.pincode = "600002";
    checkout_dto.payment_method = "CASH_ON_DELIVERY";
    auto order = order_service.Checkout(buyer_id, checkout_dto);

    // Stock should be deducted
    auto prod_after_order = prod_repo->FindById(1);
    EXPECT_EQ(prod_after_order->stock_qty, initial_stock - 2);

    // Buyer cancels order
    auto cancelled_order = order_service.CancelOrder(order.id, buyer_id, false);
    EXPECT_EQ(cancelled_order.status, "CANCELLED");

    // Reserved inventory restored to stock
    auto prod_after_cancel = prod_repo->FindById(1);
    EXPECT_EQ(prod_after_cancel->stock_qty, initial_stock);
}
