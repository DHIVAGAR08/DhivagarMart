#include <cstdint>
#include <gtest/gtest.h>
#include "repository/UserRepository.h"
#include "repository/ProductRepository.h"
#include "repository/CartRepository.h"
#include "repository/OrderRepository.h"
#include "plugin/DatabasePlugin.h"

using namespace dhivagar::dhivagarmart;

class RepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize pool with .env configuration
        plugin::DatabasePlugin::InitAndStart();
    }
};

TEST_F(RepositoryTest, DatabaseHealthCheck) {
    EXPECT_TRUE(plugin::DatabasePlugin::CheckHealth());
}

TEST_F(RepositoryTest, UserFindAdmin) {
    repository::UserRepository repo;
    auto admin_opt = repo.FindByEmail("admin@dhivagarmart.com");
    ASSERT_TRUE(admin_opt.has_value());
    EXPECT_EQ(admin_opt->name, "System Administrator");
    EXPECT_EQ(admin_opt->role, model::UserRole::kAdmin);
}

TEST_F(RepositoryTest, ProductFindAllAndFilter) {
    repository::ProductRepository repo;
    auto all_prods = repo.FindAll(std::nullopt, std::nullopt);
    EXPECT_GE(all_prods.size(), 6u);

    auto mobile_prods = repo.FindAll("Mobiles", std::nullopt);
    EXPECT_GE(mobile_prods.size(), 2u);
    for (const auto& p : mobile_prods) {
        EXPECT_EQ(p.category, "Mobiles");
    }

    auto search_prods = repo.FindAll(std::nullopt, "Laptop");
    EXPECT_GE(search_prods.size(), 1u);
}

TEST_F(RepositoryTest, CartLifecycle) {
    repository::CartRepository cart_repo;
    int64_t test_user_id = 3; // Buyer
    int64_t test_product_id = 1;

    cart_repo.ClearCart(test_user_id);
    auto initial_items = cart_repo.GetCartItems(test_user_id);
    EXPECT_TRUE(initial_items.empty());

    cart_repo.AddOrIncrementItem(test_user_id, test_product_id, 2);
    auto items = cart_repo.GetCartItems(test_user_id);
    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0].product_id, test_product_id);
    EXPECT_EQ(items[0].quantity, 2);

    cart_repo.UpdateQuantity(test_user_id, test_product_id, 5);
    items = cart_repo.GetCartItems(test_user_id);
    ASSERT_EQ(items.size(), 1u);
    EXPECT_EQ(items[0].quantity, 5);

    cart_repo.RemoveItem(test_user_id, test_product_id);
    items = cart_repo.GetCartItems(test_user_id);
    EXPECT_TRUE(items.empty());
}

TEST_F(RepositoryTest, OrderLifecycleAndQueryCompatibility) {
    repository::OrderRepository order_repo;
    int64_t test_user_id = 3; // Seeded Buyer
    int64_t test_product_id = 1;

    // RAII guard guarantees cleanup of this exact test order even if assertions fail or throw
    struct OrderCleanupGuard {
        int64_t order_id{0};
        ~OrderCleanupGuard() {
            if (order_id > 0) {
                try {
                    auto conn = plugin::DatabasePlugin::GetConnection();
                    pqxx::work clean_tx(*conn);
                    clean_tx.exec_params("DELETE FROM order_items WHERE order_id = $1;", order_id);
                    clean_tx.exec_params("DELETE FROM orders WHERE id = $1;", order_id);
                    clean_tx.commit();
                } catch (...) {
                    // Suppress exceptions in destructor to ensure safe stack unwinding
                }
            }
        }
    } cleanup_guard;

    auto conn = plugin::DatabasePlugin::GetConnection();

    // Create an order in a transaction
    pqxx::work tx(*conn);
    int64_t order_id = order_repo.CreateOrderInTransaction(
        tx, test_user_id, 12999900, model::OrderStatus::kConfirmed,
        "CASH_ON_DELIVERY", "PENDING", "42 Tech St, Chennai", "+91 9876543210", "John Buyer"
    );
    cleanup_guard.order_id = order_id; // Registered with guard immediately

    order_repo.CreateOrderItemInTransaction(tx, order_id, test_product_id, 1, 12999900);
    tx.commit();

    // Verify FindById executes cleanly without schema column errors
    auto fetched = order_repo.FindById(order_id);
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->id, order_id);
    EXPECT_EQ(fetched->buyer_id, test_user_id);
    EXPECT_EQ(fetched->status, model::OrderStatus::kConfirmed);
    EXPECT_EQ(fetched->payment_method, "CASH_ON_DELIVERY");
    EXPECT_EQ(fetched->delivery_address, "42 Tech St, Chennai");
    ASSERT_EQ(fetched->items.size(), 1u);
    EXPECT_EQ(fetched->items[0].product_id, test_product_id);
    EXPECT_EQ(fetched->items[0].quantity, 1);

    // Verify FindByBuyerId contains this exact test order ID
    auto buyer_orders = order_repo.FindByBuyerId(test_user_id);
    EXPECT_FALSE(buyer_orders.empty());
    bool found_in_buyer = false;
    for (const auto& ord : buyer_orders) {
        if (ord.id == order_id) {
            found_in_buyer = true;
            EXPECT_EQ(ord.buyer_id, test_user_id);
            EXPECT_EQ(ord.status, model::OrderStatus::kConfirmed);
            break;
        }
    }
    EXPECT_TRUE(found_in_buyer);

    // Verify FindAll contains this exact test order ID
    auto all_orders = order_repo.FindAll();
    EXPECT_FALSE(all_orders.empty());
    bool found_in_all = false;
    for (const auto& ord : all_orders) {
        if (ord.id == order_id) {
            found_in_all = true;
            EXPECT_EQ(ord.buyer_id, test_user_id);
            break;
        }
    }
    EXPECT_TRUE(found_in_all);
}

