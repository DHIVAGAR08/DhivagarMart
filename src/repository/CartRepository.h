#pragma once

#include <cstdint>

#include "repository/ICartRepository.h"

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Concrete PostgreSQL implementation of ICartRepository.
 */
class CartRepository : public ICartRepository {
public:
    CartRepository() = default;
    ~CartRepository() override = default;

    std::vector<model::CartItem> GetCartItems(int64_t user_id, const std::string& request_id = "") override;
    void AddOrIncrementItem(int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id = "") override;
    bool UpdateQuantity(int64_t user_id, int64_t product_id, int32_t quantity, const std::string& request_id = "") override;
    bool RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id = "") override;
    void ClearCart(int64_t user_id, const std::string& request_id = "") override;
    void ClearCartInTransaction(pqxx::work& tx, int64_t user_id, const std::string& request_id = "") override;
};

} // namespace dhivagar::dhivagarmart::repository
