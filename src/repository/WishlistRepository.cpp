#include <cstdint>
#include "repository/WishlistRepository.h"
#include "plugin/DatabasePlugin.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::repository {

std::vector<model::WishlistItem> WishlistRepository::GetWishlistItems(int64_t user_id, const std::string& request_id) {
    spdlog::debug("[{}] WishlistRepository::GetWishlistItems for user_id={}", request_id, user_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT w.id, w.user_id, w.product_id, w.created_at, "
        "       p.name AS product_name, p.description AS product_description, "
        "       p.category AS product_category, p.image_url AS product_image_url, "
        "       p.price_cents, p.stock_qty "
        "FROM wishlist_items w "
        "JOIN products p ON w.product_id = p.id "
        "WHERE w.user_id = $1 "
        "ORDER BY w.id DESC;",
        user_id
    );

    std::vector<model::WishlistItem> items;
    items.reserve(res.size());
    for (const auto& row : res) {
        model::WishlistItem item;
        item.id = row["id"].as<int64_t>();
        item.user_id = row["user_id"].as<int64_t>();
        item.product_id = row["product_id"].as<int64_t>();
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

bool WishlistRepository::AddItem(int64_t user_id, int64_t product_id, const std::string& request_id) {
    spdlog::debug("[{}] WishlistRepository::AddItem user_id={} product_id={}", request_id, user_id, product_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "INSERT INTO wishlist_items (user_id, product_id) "
        "VALUES ($1, $2) "
        "ON CONFLICT (user_id, product_id) DO NOTHING;",
        user_id, product_id
    );
    tx.commit();
    return res.affected_rows() > 0;
}

bool WishlistRepository::RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id) {
    spdlog::debug("[{}] WishlistRepository::RemoveItem user_id={} product_id={}", request_id, user_id, product_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "DELETE FROM wishlist_items WHERE user_id = $1 AND product_id = $2;",
        user_id, product_id
    );
    tx.commit();
    return res.affected_rows() > 0;
}

bool WishlistRepository::IsInWishlist(int64_t user_id, int64_t product_id, const std::string& request_id) {
    spdlog::debug("[{}] WishlistRepository::IsInWishlist user_id={} product_id={}", request_id, user_id, product_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT 1 FROM wishlist_items WHERE user_id = $1 AND product_id = $2 LIMIT 1;",
        user_id, product_id
    );
    return !res.empty();
}

} // namespace dhivagar::dhivagarmart::repository
