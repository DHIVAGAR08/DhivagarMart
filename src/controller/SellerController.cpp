#include <cstdint>
#include "controller/SellerController.h"
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

SellerController::SellerController() {
    auto product_repo = std::make_shared<repository::ProductRepository>();
    auto order_repo = std::make_shared<repository::OrderRepository>();
    seller_service_ = std::make_shared<service::SellerService>(product_repo, order_repo);
}

void SellerController::GetProducts(const drogon::HttpRequestPtr &req, 
                                   std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t seller_id = req->session()->get<int64_t>("user_id");
        auto products = seller_service_->GetSellerProducts(seller_id, request_id);

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

void SellerController::CreateProduct(const drogon::HttpRequestPtr &req, 
                                     std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t seller_id = req->session()->get<int64_t>("user_id");
        auto json_body = nlohmann::json::parse(req->body());
        dto::ProductRequestDto dto = json_body.get<dto::ProductRequestDto>();

        auto created = seller_service_->CreateProduct(seller_id, dto, request_id);

        nlohmann::json data;
        to_json(data, created);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k201Created));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void SellerController::UpdateProduct(const drogon::HttpRequestPtr &req, 
                                     std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                     int64_t product_id) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t seller_id = req->session()->get<int64_t>("user_id");
        auto json_body = nlohmann::json::parse(req->body());
        dto::ProductRequestDto dto = json_body.get<dto::ProductRequestDto>();

        auto updated = seller_service_->UpdateProduct(seller_id, product_id, dto, request_id);

        nlohmann::json data;
        to_json(data, updated);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void SellerController::DeleteProduct(const drogon::HttpRequestPtr &req, 
                                     std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                     int64_t product_id) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t seller_id = req->session()->get<int64_t>("user_id");
        seller_service_->DeleteProduct(seller_id, product_id, request_id);
        callback(util::JsonUtil::CreateSuccessResponse(nlohmann::json{{"deleted", true}}, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void SellerController::GetOrders(const drogon::HttpRequestPtr &req, 
                                 std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t seller_id = req->session()->get<int64_t>("user_id");
        auto orders = seller_service_->GetSellerOrders(seller_id, request_id);

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

void SellerController::UpdateOrderStatus(const drogon::HttpRequestPtr &req, 
                                         std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                         int64_t order_id) {
    std::string request_id = GetRequestId(req);
    try {
        int64_t seller_id = req->session()->get<int64_t>("user_id");
        auto json_body = nlohmann::json::parse(req->body());
        dto::UpdateOrderStatusRequestDto dto = json_body.get<dto::UpdateOrderStatusRequestDto>();

        auto updated = seller_service_->UpdateOrderStatus(seller_id, order_id, dto.status, request_id);

        nlohmann::json data;
        to_json(data, updated);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
