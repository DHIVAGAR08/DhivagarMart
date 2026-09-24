#pragma once

#include <cstdint>

#include "repository/IReviewRepository.h"

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Concrete PostgreSQL implementation of IReviewRepository.
 */
class ReviewRepository : public IReviewRepository {
public:
    ReviewRepository() = default;
    ~ReviewRepository() override = default;

    model::Review Create(const model::Review& review, const std::string& request_id = "") override;
    std::vector<model::Review> FindByProductId(int64_t product_id, const std::string& request_id = "") override;
    bool HasUserReviewedProduct(int64_t user_id, int64_t product_id, const std::string& request_id = "") override;
};

} // namespace dhivagar::dhivagarmart::repository
