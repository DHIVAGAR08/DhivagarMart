#pragma once

#include <cstdint>

#include "model/Review.h"
#include <vector>
#include <string>
#include <memory>

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Interface for Review data access operations.
 */
class IReviewRepository {
public:
    virtual ~IReviewRepository() = default;

    virtual model::Review Create(const model::Review& review, const std::string& request_id = "") = 0;
    virtual std::vector<model::Review> FindByProductId(int64_t product_id, const std::string& request_id = "") = 0;
    virtual bool HasUserReviewedProduct(int64_t user_id, int64_t product_id, const std::string& request_id = "") = 0;
};

} // namespace dhivagar::dhivagarmart::repository
