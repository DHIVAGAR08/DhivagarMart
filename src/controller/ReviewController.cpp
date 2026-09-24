#include <cstdint>
#include "controller/ReviewController.h"
#include "repository/ReviewRepository.h"
#include "repository/OrderRepository.h"
#include "repository/ProductRepository.h"
#include "exception/GlobalExceptionHandler.h"
#include "util/JsonUtil.h"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::controller {

static std::string GetRequestId(const drogon::HttpRequestPtr &req) {
    if (req->getAttributes()->find("request_id")) {
        return req->getAttributes()->get<std::string>("request_id");
    }
    return "req-unknown";
}

ReviewController::ReviewController() {
    auto review_repo = std::make_shared<repository::ReviewRepository>();
    auto order_repo = std::make_shared<repository::OrderRepository>();
    auto product_repo = std::make_shared<repository::ProductRepository>();
    review_service_ = std::make_shared<service::ReviewService>(review_repo, order_repo, product_repo);
}

void ReviewController::AddReview(const drogon::HttpRequestPtr &req, 
                                std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                int64_t product_id) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t user_id = req->session()->get<int64_t>("user_id");
        std::string user_name = req->session()->get<std::string>("user_name");

        auto json_body = nlohmann::json::parse(req->body());
        dto::CreateReviewRequestDto dto = json_body.get<dto::CreateReviewRequestDto>();

        auto review = review_service_->AddReview(user_id, product_id, user_name, dto, request_id);

        nlohmann::json data;
        to_json(data, review);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k201Created));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void ReviewController::GetProductReviews(const drogon::HttpRequestPtr &req, 
                                        std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                        int64_t product_id) {
    std::string request_id = GetRequestId(req);
    try {
        auto reviews = review_service_->GetProductReviews(product_id, request_id);

        nlohmann::json array_data = nlohmann::json::array();
        for (const auto& rev : reviews) {
            nlohmann::json j;
            to_json(j, rev);
            array_data.push_back(j);
        }

        callback(util::JsonUtil::CreateSuccessResponse(array_data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
