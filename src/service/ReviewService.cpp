#include <cstdint>
#include "service/ReviewService.h"
#include "exception/AppException.h"
#include "util/ValidationUtil.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

ReviewService::ReviewService(std::shared_ptr<repository::IReviewRepository> review_repo,
                             std::shared_ptr<repository::IOrderRepository> order_repo,
                             std::shared_ptr<repository::IProductRepository> product_repo)
    : review_repo_(std::move(review_repo)),
      order_repo_(std::move(order_repo)),
      product_repo_(std::move(product_repo)) {}

dto::ReviewResponseDto ReviewService::AddReview(
    int64_t user_id, int64_t product_id,
    const std::string& user_name,
    const dto::CreateReviewRequestDto& req,
    const std::string& request_id) {

    spdlog::info("[{}] ReviewService::AddReview user={} product={} rating={}", 
                 request_id, user_id, product_id, req.rating);

    // 1. Validation at top
    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }
    if (!util::ValidationUtil::IsValidRating(req.rating)) {
        throw exception::ValidationException("Rating must be an integer between 1 and 5");
    }
    std::string clean_comment = util::ValidationUtil::Trim(req.comment);
    if (clean_comment.empty()) {
        throw exception::ValidationException("Review comment cannot be empty");
    }

    // 2. Product existence check
    auto product_opt = product_repo_->FindById(product_id, request_id);
    if (!product_opt.has_value()) {
        throw exception::NotFoundException("Product does not exist");
    }

    // 3. Duplicate review prevention
    if (review_repo_->HasUserReviewedProduct(user_id, product_id, request_id)) {
        throw exception::ConflictException("You have already submitted a review for this product");
    }

    // 4. Verified Purchase Enforcement: User can review only after completed/confirmed order
    if (!order_repo_->HasBuyerCompletedPurchaseOfProduct(user_id, product_id, request_id)) {
        throw exception::AuthorizationException("Only customers who have purchased this product may submit a review");
    }

    // 5. Create and persist review
    model::Review review;
    review.product_id = product_id;
    review.user_id = user_id;
    review.user_name = user_name;
    review.rating = req.rating;
    review.comment = util::ValidationUtil::EscapeHtml(clean_comment);

    auto created = review_repo_->Create(review, request_id);
    return dto::ReviewResponseDto::FromModel(created);
}

std::vector<dto::ReviewResponseDto> ReviewService::GetProductReviews(int64_t product_id, const std::string& request_id) {
    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }

    auto reviews = review_repo_->FindByProductId(product_id, request_id);
    std::vector<dto::ReviewResponseDto> dtos;
    dtos.reserve(reviews.size());
    for (const auto& rev : reviews) {
        dtos.push_back(dto::ReviewResponseDto::FromModel(rev));
    }
    return dtos;
}

} // namespace dhivagar::dhivagarmart::service
