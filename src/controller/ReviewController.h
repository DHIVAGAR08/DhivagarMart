#pragma once

#include <cstdint>

#include <drogon/HttpController.h>
#include "service/ReviewService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller handling customer ratings and verified product reviews.
 */
class ReviewController : public drogon::HttpController<ReviewController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ReviewController::AddReview, "/api/v1/products/{1}/reviews", drogon::Post, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(ReviewController::GetProductReviews, "/api/v1/products/{1}/reviews", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter");
    METHOD_LIST_END

    ReviewController();

    void AddReview(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);
    void GetProductReviews(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);

private:
    std::shared_ptr<service::ReviewService> review_service_;
};

} // namespace dhivagar::dhivagarmart::controller
