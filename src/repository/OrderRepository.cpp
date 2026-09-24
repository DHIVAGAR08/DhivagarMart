#include <cstdint>
#include "repository/OrderRepository.h"
#include "plugin/DatabasePlugin.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::repository {

std::vector<model::OrderItem> OrderRepository::FetchOrderItems(pqxx::nontransaction& ntx, int64_t order_id) {
    auto res = ntx.exec_params(
        "SELECT oi.id, oi.order_id, oi.product_id, oi.quantity, oi.unit_price_cents, oi.created_at, "
        "       p.name AS product_name "
        "FROM order_items oi "
        "LEFT JOIN products p ON oi.product_id = p.id "
        "WHERE oi.order_id = $1 "
        "ORDER BY oi.id ASC;",
        order_id
    );

    std::vector<model::OrderItem> items;
    items.reserve(res.size());
    for (const auto& row : res) {
        model::OrderItem item;
        item.id = row["id"].as<int64_t>();
        item.order_id = row["order_id"].as<int64_t>();
        item.product_id = row["product_id"].as<int64_t>();
        item.product_name = row["product_name"].is_null() ? "Discontinued Product" : row["product_name"].as<std::string>();
        item.quantity = row["quantity"].as<int32_t>();
        item.unit_price = model::Money::FromCents(row["unit_price_cents"].as<int64_t>());
        item.created_at = row["created_at"].as<std::string>();
        items.push_back(item);
    }
    return items;
}

int64_t OrderRepository::CreateOrderInTransaction(pqxx::work& tx, int64_t buyer_id, 
                                                 int64_t total_amount_cents, 
                                                 model::OrderStatus status, 
                                                 const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::CreateOrderInTransaction buyer_id={} total={}", 
                  request_id, buyer_id, total_amount_cents);

    std::string status_str = model::OrderStatusToString(status);
    auto res = tx.exec_params(
        "INSERT INTO orders (buyer_id, status, total_amount_cents) "
        "VALUES ($1, $2, $3) "
        "RETURNING id;",
        buyer_id, status_str, total_amount_cents
    );
    if (res.empty()) {
        throw std::runtime_error("Failed to insert order header");
    }
    return res[0]["id"].as<int64_t>();
}

void OrderRepository::CreateOrderItemInTransaction(pqxx::work& tx, int64_t order_id, 
                                                  int64_t product_id, int32_t quantity, 
                                                  int64_t unit_price_cents, 
                                                  const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::CreateOrderItemInTransaction order_id={} product_id={} qty={}", 
                  request_id, order_id, product_id, quantity);

    tx.exec_params(
        "INSERT INTO order_items (order_id, product_id, quantity, unit_price_cents) "
        "VALUES ($1, $2, $3, $4);",
        order_id, product_id, quantity, unit_price_cents
    );
}

std::optional<model::Order> OrderRepository::FindById(int64_t id, const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::FindById id={}", request_id, id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT o.id, o.buyer_id, o.status, o.total_amount_cents, o.created_at, "
        "       u.name AS buyer_name, u.email AS buyer_email "
        "FROM orders o "
        "JOIN users u ON o.buyer_id = u.id "
        "WHERE o.id = $1;",
        id
    );

    if (res.empty()) {
        return std::nullopt;
    }

    const auto& row = res[0];
    model::Order order;
    order.id = row["id"].as<int64_t>();
    order.buyer_id = row["buyer_id"].as<int64_t>();
    order.buyer_name = row["buyer_name"].as<std::string>();
    order.buyer_email = row["buyer_email"].as<std::string>();
    order.status = model::StringToOrderStatus(row["status"].as<std::string>());
    order.total_amount = model::Money::FromCents(row["total_amount_cents"].as<int64_t>());
    order.created_at = row["created_at"].as<std::string>();
    order.items = FetchOrderItems(ntx, order.id);

    return order;
}

std::vector<model::Order> OrderRepository::FindByBuyerId(int64_t buyer_id, const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::FindByBuyerId buyer_id={}", request_id, buyer_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT o.id, o.buyer_id, o.status, o.total_amount_cents, o.created_at, "
        "       u.name AS buyer_name, u.email AS buyer_email "
        "FROM orders o "
        "JOIN users u ON o.buyer_id = u.id "
        "WHERE o.buyer_id = $1 "
        "ORDER BY o.id DESC;",
        buyer_id
    );

    std::vector<model::Order> orders;
    orders.reserve(res.size());
    for (const auto& row : res) {
        model::Order order;
        order.id = row["id"].as<int64_t>();
        order.buyer_id = row["buyer_id"].as<int64_t>();
        order.buyer_name = row["buyer_name"].as<std::string>();
        order.buyer_email = row["buyer_email"].as<std::string>();
        order.status = model::StringToOrderStatus(row["status"].as<std::string>());
        order.total_amount = model::Money::FromCents(row["total_amount_cents"].as<int64_t>());
        order.created_at = row["created_at"].as<std::string>();
        order.items = FetchOrderItems(ntx, order.id);
        orders.push_back(order);
    }
    return orders;
}

std::vector<model::Order> OrderRepository::FindBySellerId(int64_t seller_id, const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::FindBySellerId seller_id={}", request_id, seller_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    // Fetch distinct orders that contain at least one item from this seller
    auto res = ntx.exec_params(
        "SELECT DISTINCT o.id, o.buyer_id, o.status, o.total_amount_cents, o.created_at, "
        "       u.name AS buyer_name, u.email AS buyer_email "
        "FROM orders o "
        "JOIN users u ON o.buyer_id = u.id "
        "JOIN order_items oi ON o.id = oi.order_id "
        "JOIN products p ON oi.product_id = p.id "
        "WHERE p.seller_id = $1 "
        "ORDER BY o.id DESC;",
        seller_id
    );

    std::vector<model::Order> orders;
    orders.reserve(res.size());
    for (const auto& row : res) {
        model::Order order;
        order.id = row["id"].as<int64_t>();
        order.buyer_id = row["buyer_id"].as<int64_t>();
        order.buyer_name = row["buyer_name"].as<std::string>();
        order.buyer_email = row["buyer_email"].as<std::string>();
        order.status = model::StringToOrderStatus(row["status"].as<std::string>());
        order.total_amount = model::Money::FromCents(row["total_amount_cents"].as<int64_t>());
        order.created_at = row["created_at"].as<std::string>();
        order.items = FetchOrderItems(ntx, order.id);
        orders.push_back(order);
    }
    return orders;
}

std::vector<model::Order> OrderRepository::FindAll(const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::FindAll", request_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT o.id, o.buyer_id, o.status, o.total_amount_cents, o.created_at, "
        "       u.name AS buyer_name, u.email AS buyer_email "
        "FROM orders o "
        "JOIN users u ON o.buyer_id = u.id "
        "ORDER BY o.id DESC;"
    );

    std::vector<model::Order> orders;
    orders.reserve(res.size());
    for (const auto& row : res) {
        model::Order order;
        order.id = row["id"].as<int64_t>();
        order.buyer_id = row["buyer_id"].as<int64_t>();
        order.buyer_name = row["buyer_name"].as<std::string>();
        order.buyer_email = row["buyer_email"].as<std::string>();
        order.status = model::StringToOrderStatus(row["status"].as<std::string>());
        order.total_amount = model::Money::FromCents(row["total_amount_cents"].as<int64_t>());
        order.created_at = row["created_at"].as<std::string>();
        order.items = FetchOrderItems(ntx, order.id);
        orders.push_back(order);
    }
    return orders;
}

bool OrderRepository::UpdateStatus(int64_t order_id, model::OrderStatus status, const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::UpdateStatus order_id={} status={}", 
                  request_id, order_id, model::OrderStatusToString(status));
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    std::string status_str = model::OrderStatusToString(status);
    auto res = tx.exec_params(
        "UPDATE orders SET status = $1 WHERE id = $2;",
        status_str, order_id
    );
    tx.commit();
    return res.affected_rows() > 0;
}

bool OrderRepository::HasBuyerCompletedPurchaseOfProduct(int64_t buyer_id, int64_t product_id, const std::string& request_id) {
    spdlog::debug("[{}] OrderRepository::HasBuyerCompletedPurchaseOfProduct buyer_id={} product_id={}", 
                  request_id, buyer_id, product_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    // Eligible statuses for verified review: DELIVERED, SHIPPED, or CONFIRMED
    auto res = ntx.exec_params(
        "SELECT 1 FROM orders o "
        "JOIN order_items oi ON o.id = oi.order_id "
        "WHERE o.buyer_id = $1 AND oi.product_id = $2 AND o.status IN ('DELIVERED', 'SHIPPED', 'CONFIRMED') "
        "LIMIT 1;",
        buyer_id, product_id
    );
    return !res.empty();
}

} // namespace dhivagar::dhivagarmart::repository
