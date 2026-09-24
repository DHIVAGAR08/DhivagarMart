#include <cstdint>
#include "controller/OrderController.h"
#include "repository/OrderRepository.h"
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

OrderController::OrderController() {
    auto order_repo = std::make_shared<repository::OrderRepository>();
    auto cart_repo = std::make_shared<repository::CartRepository>();
    auto product_repo = std::make_shared<repository::ProductRepository>();
    order_service_ = std::make_shared<service::OrderService>(order_repo, cart_repo, product_repo);
}

void OrderController::Checkout(const drogon::HttpRequestPtr &req, 
                              std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t buyer_id = req->session()->get<int64_t>("user_id");
        dto::CreateOrderRequestDto dto;
        if (!req->body().empty()) {
            auto j = nlohmann::json::parse(req->body());
            dto = j.get<dto::CreateOrderRequestDto>();
        }

        auto order = order_service_->Checkout(buyer_id, dto, request_id);

        nlohmann::json data;
        to_json(data, order);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k201Created));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void OrderController::GetOrders(const drogon::HttpRequestPtr &req, 
                               std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t buyer_id = req->session()->get<int64_t>("user_id");
        auto orders = order_service_->GetBuyerOrders(buyer_id, request_id);

        nlohmann::json array_data = nlohmann::json::array();
        for (const auto& ord : orders) {
            nlohmann::json j;
            to_json(j, ord);
            array_data.push_back(j);
        }

        callback(util::JsonUtil::CreateSuccessResponse(array_data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void OrderController::GetOrderById(const drogon::HttpRequestPtr &req, 
                                  std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                  int64_t order_id) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t user_id = req->session()->get<int64_t>("user_id");
        std::string role = req->session()->get<std::string>("role");
        bool is_admin = (role == "ADMIN");

        auto order = order_service_->GetOrderById(order_id, user_id, is_admin, request_id);

        nlohmann::json data;
        to_json(data, order);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
