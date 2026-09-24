#include <cstdint>
#include "repository/CartRepository.h"
#include "plugin/DatabasePlugin.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::repository {

std::vector<model::CartItem> CartRepository::GetCartItems(int64_t user_id, const std::string& request_id) {
    spdlog::debug("[{}] CartRepository::GetCartItems for user_id={}", request_id, user_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT c.id, c.user_id, c.product_id, c.quantity, c.created_at, "
        "       p.name AS product_name, p.description AS product_description, "
        "       p.category AS product_category, p.image_url AS product_image_url, "
        "       p.price_cents, p.stock_qty "
        "FROM cart_items c "
        "JOIN products p ON c.product_id = p.id "
        "WHERE c.user_id = $1 "
        "ORDER BY c.id ASC;",
        user_id
    );

    std::vector<model::CartItem> items;
    items.reserve(res.size());
    for (const auto& row : res) {
        model::CartItem item;
        item.id = row["id"].as<int64_t>();
        item.user_id = row["user_id"].as<int64_t>();
        item.product_id = row["product_id"].as<int64_t>();
        item.quantity = row["quantity"].as<int32_t>();
        item.created_at = row["created_at"].as<std::string>();
        item.product_name = row["product_name"].as<std::string>();
        item.product_description = row["product_description"].as<std::string>();
        item.product_category = row["product_category"].as<std::string>();
        item.product_image_url = row["product_image_url"].as<std::string>();
        item.product_price = model::Money::FromCents(row["price_cents"].as<int64_t>());
        item.available_stock = row["stock_qty"].as<int32_t>();
        items.push_back(item);
    }
    return items;
}

void CartRepository::AddOrIncrementItem(int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id) {
    spdlog::debug("[{}] CartRepository::AddOrIncrementItem user_id={} product_id={} qty={}", 
                  request_id, user_id, product_id, quantity);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    tx.exec_params(
        "INSERT INTO cart_items (user_id, product_id, quantity) "
        "VALUES ($1, $2, $3) "
        "ON CONFLICT (user_id, product_id) "
        "DO UPDATE SET quantity = cart_items.quantity + EXCLUDED.quantity;",
        user_id, product_id, quantity
    );
    tx.commit();
}

bool CartRepository::UpdateQuantity(int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id) {
    spdlog::debug("[{}] CartRepository::UpdateQuantity user_id={} product_id={} qty={}", 
                  request_id, user_id, product_id, quantity);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "UPDATE cart_items SET quantity = $1 WHERE user_id = $2 AND product_id = $3;",
        quantity, user_id, product_id
    );
    tx.commit();
    return res.affected_rows() > 0;
}

bool CartRepository::RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id) {
    spdlog::debug("[{}] CartRepository::RemoveItem user_id={} product_id={}", request_id, user_id, product_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "DELETE FROM cart_items WHERE user_id = $1 AND product_id = $2;",
        user_id, product_id
    );
    tx.commit();
    return res.affected_rows() > 0;
}

void CartRepository::ClearCart(int64_t user_id, const std::string& request_id) {
    spdlog::debug("[{}] CartRepository::ClearCart user_id={}", request_id, user_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);
    tx.exec_params("DELETE FROM cart_items WHERE user_id = $1;", user_id);
    tx.commit();
}

void CartRepository::ClearCartInTransaction(pqxx::work& tx, int64_t user_id, const std::string& request_id) {
    spdlog::debug("[{}] CartRepository::ClearCartInTransaction user_id={}", request_id, user_id);
    tx.exec_params("DELETE FROM cart_items WHERE user_id = $1;", user_id);
}

} // namespace dhivagar::dhivagarmart::repository
