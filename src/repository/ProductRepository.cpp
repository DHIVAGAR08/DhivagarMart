#include <cstdint>
#include "repository/ProductRepository.h"
#include "plugin/DatabasePlugin.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::repository {

static model::Product RowToProduct(const pqxx::row_ref& row) {
    model::Product p;
    p.id = row["id"].as<int64_t>();
    p.seller_id = row["seller_id"].as<int64_t>();
    p.name = row["name"].as<std::string>();
    p.description = row["description"].as<std::string>();
    p.price = model::Money::FromCents(row["price_cents"].as<int64_t>());
    p.stock_qty = row["stock_qty"].as<int32_t>();
    p.category = row["category"].as<std::string>();
    p.image_url = row["image_url"].as<std::string>();
    p.created_at = row["created_at"].as<std::string>();
    return p;
}

model::Product ProductRepository::Create(const model::Product& product, const std::string& request_id) {
    spdlog::debug("[{}] ProductRepository::Create: {}", request_id, product.name);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "INSERT INTO products (seller_id, name, description, price_cents, stock_qty, category, image_url) "
        "VALUES ($1, $2, $3, $4, $5, $6, $7) "
        "RETURNING id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at;",
        product.seller_id, product.name, product.description, product.price.GetCents(),
        product.stock_qty, product.category, product.image_url
    );
    tx.commit();

    if (res.empty()) {
        throw std::runtime_error("Failed to insert product record");
    }
    return RowToProduct(res[0]);
}

std::optional<model::Product> ProductRepository::FindById(int64_t id, const std::string& request_id) {
    spdlog::debug("[{}] ProductRepository::FindById: {}", request_id, id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at "
        "FROM products WHERE id = $1;",
        id
    );

    if (res.empty()) {
        return std::nullopt;
    }
    return RowToProduct(res[0]);
}

std::vector<model::Product> ProductRepository::FindAll(
    const std::optional<std::string>& category,
    const std::optional<std::string>& search,
    const std::string& request_id) {
    
    spdlog::debug("[{}] ProductRepository::FindAll category={} search={}", 
                  request_id, category.value_or("all"), search.value_or("none"));
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    pqxx::result res;
    if (category.has_value() && !category->empty() && search.has_value() && !search->empty()) {
        std::string pattern = "%" + *search + "%";
        res = ntx.exec_params(
            "SELECT id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at "
            "FROM products "
            "WHERE category = $1 AND (name ILIKE $2 OR description ILIKE $2) "
            "ORDER BY id ASC;",
            *category, pattern
        );
    } else if (category.has_value() && !category->empty()) {
        res = ntx.exec_params(
            "SELECT id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at "
            "FROM products "
            "WHERE category = $1 "
            "ORDER BY id ASC;",
            *category
        );
    } else if (search.has_value() && !search->empty()) {
        std::string pattern = "%" + *search + "%";
        res = ntx.exec_params(
            "SELECT id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at "
            "FROM products "
            "WHERE name ILIKE $1 OR description ILIKE $1 "
            "ORDER BY id ASC;",
            pattern
        );
    } else {
        res = ntx.exec_params(
            "SELECT id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at "
            "FROM products "
            "ORDER BY id ASC;"
        );
    }

    std::vector<model::Product> products;
    products.reserve(res.size());
    for (const auto& row : res) {
        products.push_back(RowToProduct(row));
    }
    return products;
}

std::vector<model::Product> ProductRepository::FindBySellerId(int64_t seller_id, const std::string& request_id) {
    spdlog::debug("[{}] ProductRepository::FindBySellerId: {}", request_id, seller_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at "
        "FROM products WHERE seller_id = $1 ORDER BY id ASC;",
        seller_id
    );

    std::vector<model::Product> products;
    products.reserve(res.size());
    for (const auto& row : res) {
        products.push_back(RowToProduct(row));
    }
    return products;
}

model::Product ProductRepository::Update(const model::Product& product, const std::string& request_id) {
    spdlog::debug("[{}] ProductRepository::Update: id={} seller={}", request_id, product.id, product.seller_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "UPDATE products "
        "SET name = $1, description = $2, price_cents = $3, stock_qty = $4, category = $5, image_url = $6 "
        "WHERE id = $7 AND seller_id = $8 "
        "RETURNING id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at;",
        product.name, product.description, product.price.GetCents(),
        product.stock_qty, product.category, product.image_url,
        product.id, product.seller_id
    );
    tx.commit();

    if (res.empty()) {
        throw std::runtime_error("Product update failed: product not found or unauthorized");
    }
    return RowToProduct(res[0]);
}

bool ProductRepository::Delete(int64_t id, int64_t seller_id, const std::string& request_id) {
    spdlog::debug("[{}] ProductRepository::Delete: id={} seller={}", request_id, id, seller_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "DELETE FROM products WHERE id = $1 AND seller_id = $2;",
        id, seller_id
    );
    tx.commit();
    return res.affected_rows() > 0;
}

bool ProductRepository::AdminDelete(int64_t id, const std::string& request_id) {
    spdlog::debug("[{}] ProductRepository::AdminDelete: id={}", request_id, id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    auto res = tx.exec_params(
        "DELETE FROM products WHERE id = $1;",
        id
    );
    tx.commit();
    return res.affected_rows() > 0;
}

bool ProductRepository::ReduceStockInTransaction(pqxx::work& tx, int64_t product_id, int32_t quantity, const std::string& request_id) {
    spdlog::debug("[{}] ProductRepository::ReduceStockInTransaction: id={} qty={}", request_id, product_id, quantity);
    auto res = tx.exec_params(
        "UPDATE products "
        "SET stock_qty = stock_qty - $1 "
        "WHERE id = $2 AND stock_qty >= $1;",
        quantity, product_id
    );
    return res.affected_rows() > 0;
}

} // namespace dhivagar::dhivagarmart::repository
