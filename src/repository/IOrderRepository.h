#pragma once

#include <cstdint>

#include "model/Order.h"
#include <optional>
#include <vector>
#include <string>
#include <memory>
#include <pqxx/pqxx>

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Interface for Order data access operations.
 */
class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;

    virtual int64_t CreateOrderInTransaction(pqxx::work& tx, int64_t buyer_id, 
                                             int64_t total_amount_cents, 
                                             model::OrderStatus status, 
                                             const std::string& request_id = "") = 0;

    virtual void CreateOrderItemInTransaction(pqxx::work& tx, int64_t order_id, 
                                              int64_t product_id, int32_t quantity, 
                                              int64_t unit_price_cents, 
                                              const std::string& request_id = "") = 0;

    virtual std::optional<model::Order> FindById(int64_t id, const std::string& request_id = "") = 0;
    virtual std::vector<model::Order> FindByBuyerId(int64_t buyer_id, const std::string& request_id = "") = 0;
    virtual std::vector<model::Order> FindBySellerId(int64_t seller_id, const std::string& request_id = "") = 0;
    virtual std::vector<model::Order> FindAll(const std::string& request_id = "") = 0;
    virtual bool UpdateStatus(int64_t order_id, model::OrderStatus status, const std::string& request_id = "") = 0;
    virtual bool HasBuyerCompletedPurchaseOfProduct(int64_t buyer_id, int64_t product_id, const std::string& request_id = "") = 0;
};

} // namespace dhivagar::dhivagarmart::repository
