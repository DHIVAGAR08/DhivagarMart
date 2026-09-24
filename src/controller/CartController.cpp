#include <cstdint>
#include "controller/CartController.h"
#include "repository/CartRepository.h"
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

CartController::CartController() {
    auto cart_repo = std::make_shared<repository::CartRepository>();
    auto product_repo = std::make_shared<repository::ProductRepository>();
    cart_service_ = std::make_shared<service::CartService>(cart_repo, product_repo);
}

void CartController::GetCart(const drogon::HttpRequestPtr &req, 
                             std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t user_id = req->session()->get<int64_t>("user_id");
        auto cart = cart_service_->GetCart(user_id, request_id);

        nlohmann::json data;
        to_json(data, cart);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void CartController::AddItem(const drogon::HttpRequestPtr &req, 
                             std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t user_id = req->session()->get<int64_t>("user_id");
        auto json_body = nlohmann::json::parse(req->body());
        dto::AddCartItemRequestDto dto = json_body.get<dto::AddCartItemRequestDto>();

        auto cart = cart_service_->AddItem(user_id, dto, request_id);

        nlohmann::json data;
        to_json(data, cart);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void CartController::UpdateQuantity(const drogon::HttpRequestPtr &req, 
                                    std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                    int64_t product_id) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t user_id = req->session()->get<int64_t>("user_id");
        auto json_body = nlohmann::json::parse(req->body());
        dto::UpdateCartItemRequestDto dto = json_body.get<dto::UpdateCartItemRequestDto>();

        auto cart = cart_service_->UpdateQuantity(user_id, product_id, dto, request_id);

        nlohmann::json data;
        to_json(data, cart);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void CartController::RemoveItem(const drogon::HttpRequestPtr &req, 
                                std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                int64_t product_id) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t user_id = req->session()->get<int64_t>("user_id");
        auto cart = cart_service_->RemoveItem(user_id, product_id, request_id);

        nlohmann::json data;
        to_json(data, cart);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
