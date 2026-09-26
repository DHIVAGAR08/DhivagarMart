#pragma once

#include <cstdint>

#include "repository/IProductRepository.h"

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Concrete PostgreSQL implementation of IProductRepository.
 */
class ProductRepository : public IProductRepository {
public:
    ProductRepository() = default;
    ~ProductRepository() override = default;

    model::Product Create(const model::Product& product, const std::string& request_id = "") override;
    std::optional<model::Product> FindById(int64_t id, const std::string& request_id = "") override;
    std::vector<model::Product> FindAll(const std::optional<std::string>& category, 
                                        const std::optional<std::string>& search, 
                                        const std::string& request_id = "") override;
    std::vector<model::Product> FindBySellerId(int64_t seller_id, const std::string& request_id = "") override;
    model::Product Update(const model::Product& product, const std::string& request_id = "") override;
    bool Delete(int64_t id, int64_t seller_id, const std::string& request_id = "") override;
    bool AdminDelete(int64_t id, const std::string& request_id = "") override;
    bool ReduceStockInTransaction(pqxx::work& tx, int64_t product_id, int32_t quantity, const std::string& request_id = "") override;
    bool RestoreStockInTransaction(pqxx::work& tx, int64_t product_id, int32_t quantity, const std::string& request_id = "") override;
};

} // namespace dhivagar::dhivagarmart::repository
