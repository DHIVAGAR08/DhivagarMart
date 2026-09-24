#include <cstdint>
#include "controller/AdminController.h"
#include "repository/UserRepository.h"
#include "repository/ProductRepository.h"
#include "repository/OrderRepository.h"
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

AdminController::AdminController() {
    auto user_repo = std::make_shared<repository::UserRepository>();
    auto product_repo = std::make_shared<repository::ProductRepository>();
    auto order_repo = std::make_shared<repository::OrderRepository>();
    admin_service_ = std::make_shared<service::AdminService>(user_repo, product_repo, order_repo);
}

void AdminController::GetAllUsers(const drogon::HttpRequestPtr &req, 
                                 std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto users = admin_service_->GetAllUsers(request_id);

        nlohmann::json array_data = nlohmann::json::array();
        for (const auto& u : users) {
            nlohmann::json j;
            to_json(j, u);
            array_data.push_back(j);
        }

        callback(util::JsonUtil::CreateSuccessResponse(array_data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AdminController::GetAllProducts(const drogon::HttpRequestPtr &req, 
                                     std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto products = admin_service_->GetAllProducts(request_id);

        nlohmann::json array_data = nlohmann::json::array();
        for (const auto& p : products) {
            nlohmann::json j;
            to_json(j, p);
            array_data.push_back(j);
        }

        callback(util::JsonUtil::CreateSuccessResponse(array_data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AdminController::ModerateDeleteProduct(const drogon::HttpRequestPtr &req, 
                                           std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                           int64_t product_id) {
    std::string request_id = GetRequestId(req);
    try {
        admin_service_->ModerateDeleteProduct(product_id, request_id);
        callback(util::JsonUtil::CreateSuccessResponse(nlohmann::json{{"deleted", true}}, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AdminController::GetAllOrders(const drogon::HttpRequestPtr &req, 
                                   std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto orders = admin_service_->GetAllOrders(request_id);

        nlohmann::json array_data = nlohmann::json::array();
        for (const auto& o : orders) {
            nlohmann::json j;
            to_json(j, o);
            array_data.push_back(j);
        }

        callback(util::JsonUtil::CreateSuccessResponse(array_data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AdminController::UpdateOrderStatus(const drogon::HttpRequestPtr &req, 
                                       std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                       int64_t order_id) {
    std::string request_id = GetRequestId(req);
    try {
        auto json_body = nlohmann::json::parse(req->body());
        dto::UpdateOrderStatusRequestDto dto = json_body.get<dto::UpdateOrderStatusRequestDto>();

        auto updated = admin_service_->UpdateOrderStatus(order_id, dto.status, request_id);

        nlohmann::json data;
        to_json(data, updated);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AdminController::GetStatistics(const drogon::HttpRequestPtr &req, 
                                    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto stats = admin_service_->GetStatistics(request_id);

        nlohmann::json data;
        to_json(data, stats);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
