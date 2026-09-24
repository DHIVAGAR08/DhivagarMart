#pragma once

#include <cstdint>

#include "model/CartItem.h"
#include <vector>
#include <string>
#include <memory>
#include <pqxx/pqxx>

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Interface for Cart operations.
 */
class ICartRepository {
public:
    virtual ~ICartRepository() = default;

    virtual std::vector<model::CartItem> GetCartItems(int64_t user_id, const std::string& request_id = "") = 0;
    virtual void AddOrIncrementItem(int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id = "") = 0;
    virtual bool UpdateQuantity(int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id = "") = 0;
    virtual bool RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id = "") = 0;
    virtual void ClearCart(int64_t user_id, const std::string& request_id = "") = 0;
    virtual void ClearCartInTransaction(pqxx::work& tx, int64_t user_id, const std::string& request_id = "") = 0;
};

} // namespace dhivagar::dhivagarmart::repository
