#pragma once

#include <cstdint>

#include "model/Product.h"
#include <optional>
#include <vector>
#include <string>
#include <memory>
#include <pqxx/pqxx>

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Interface for Product data access operations.
 */
class IProductRepository {
public:
    virtual ~IProductRepository() = default;

    virtual model::Product Create(const model::Product& product, const std::string& request_id = "") = 0;
    virtual std::optional<model::Product> FindById(int64_t id, const std::string& request_id = "") = 0;
    virtual std::vector<model::Product> FindAll(const std::optional<std::string>& category, 
                                                const std::optional<std::string>& search, 
                                                const std::string& request_id = "") = 0;
    virtual std::vector<model::Product> FindBySellerId(int64_t seller_id, const std::string& request_id = "") = 0;
    virtual model::Product Update(const model::Product& product, const std::string& request_id = "") = 0;
    virtual bool Delete(int64_t id, int64_t seller_id, const std::string& request_id = "") = 0;
    virtual bool AdminDelete(int64_t id, const std::string& request_id = "") = 0;
    virtual bool ReduceStockInTransaction(pqxx::work& tx, int64_t product_id, int32_t quantity, const std::string& request_id = "") = 0;
};

} // namespace dhivagar::dhivagarmart::repository
