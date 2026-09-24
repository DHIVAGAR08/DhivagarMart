#pragma once

#include <cstdint>

#include "repository/IOrderRepository.h"

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Concrete PostgreSQL implementation of IOrderRepository.
 */
class OrderRepository : public IOrderRepository {
public:
    OrderRepository() = default;
    ~OrderRepository() override = default;

    int64_t CreateOrderInTransaction(pqxx::work& tx, int64_t buyer_id, 
                                     int64_t total_amount_cents, 
                                     model::OrderStatus status, 
                                     const std::string& request_id = "") override;

    void CreateOrderItemInTransaction(pqxx::work& tx, int64_t order_id, 
                                      int64_t product_id, int32_t quantity, 
                                      int64_t unit_price_cents, 
                                      const std::string& request_id = "") override;

    std::optional<model::Order> FindById(int64_t id, const std::string& request_id = "") override;
    std::vector<model::Order> FindByBuyerId(int64_t buyer_id, const std::string& request_id = "") override;
    std::vector<model::Order> FindBySellerId(int64_t seller_id, const std::string& request_id = "") override;
    std::vector<model::Order> FindAll(const std::string& request_id = "") override;
    bool UpdateStatus(int64_t order_id, model::OrderStatus status, const std::string& request_id = "") override;
    bool HasBuyerCompletedPurchaseOfProduct(int64_t buyer_id, int64_t product_id, const std::string& request_id = "") override;

private:
    std::vector<model::OrderItem> FetchOrderItems(pqxx::nontransaction& ntx, int64_t order_id);
};

} // namespace dhivagar::dhivagarmart::repository
