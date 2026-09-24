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
    EXPECT_GE(all_prods.size(), 10u);

    auto fruit_prods = repo.FindAll("Fruits", std::nullopt);
    EXPECT_GE(fruit_prods.size(), 2u);
    for (const auto& p : fruit_prods) {
        EXPECT_EQ(p.category, "Fruits");
    }

    auto search_prods = repo.FindAll(std::nullopt, "Rice");
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
