#pragma once

#include <cstdint>

#include "repository/IReviewRepository.h"
#include "repository/IOrderRepository.h"
#include "repository/IProductRepository.h"
#include "dto/ReviewDtos.h"
#include <memory>
#include <vector>
#include <string>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Service governing verified product reviews and customer ratings.
 */
class ReviewService {
public:
    ReviewService(std::shared_ptr<repository::IReviewRepository> review_repo,
                  std::shared_ptr<repository::IOrderRepository> order_repo,
                  std::shared_ptr<repository::IProductRepository> product_repo);

    /**
     * @brief Submits a product review after verifying the user completed an order with the item.
     * @param user_id Authenticated user ID.
     * @param product_id Product ID to review.
     * @param user_name Name of the reviewer.
     * @param req Review payload (rating 1..5, comment).
     * @param request_id Correlation ID.
     * @return Created Review response DTO.
     * @throws ValidationException on invalid rating or missing comment.
     * @throws AuthorizationException if buyer has not purchased the product.
     * @throws ConflictException if buyer already reviewed this product.
     */
    dto::ReviewResponseDto AddReview(int64_t user_id, int64_t product_id, 
                                     const std::string& user_name,
                                     const dto::CreateReviewRequestDto& req, 
                                     const std::string& request_id = "");

    /**
     * @brief Retrieves all customer reviews for a given product.
     */
    std::vector<dto::ReviewResponseDto> GetProductReviews(int64_t product_id, const std::string& request_id = "");

private:
    std::shared_ptr<repository::IReviewRepository> review_repo_;
    std::shared_ptr<repository::IOrderRepository> order_repo_;
    std::shared_ptr<repository::IProductRepository> product_repo_;
};

} // namespace dhivagar::dhivagarmart::service
