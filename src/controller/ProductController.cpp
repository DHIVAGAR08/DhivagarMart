#include <cstdint>
#include "controller/ProductController.h"
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

ProductController::ProductController() {
    auto product_repo = std::make_shared<repository::ProductRepository>();
    product_service_ = std::make_shared<service::ProductService>(product_repo);
}

void ProductController::GetProducts(const drogon::HttpRequestPtr &req, 
                                   std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        std::optional<std::string> category;
        std::optional<std::string> search;

        auto cat_param = req->getParameter("category");
        if (!cat_param.empty()) {
            category = cat_param;
        }

        auto search_param = req->getParameter("search");
        if (!search_param.empty()) {
            search = search_param;
        }

        auto products = product_service_->GetProducts(category, search, request_id);

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

void ProductController::GetProductById(const drogon::HttpRequestPtr &req, 
                                       std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
                                       int64_t id) {
    std::string request_id = GetRequestId(req);
    try {
        auto product = product_service_->GetProductById(id, request_id);
        nlohmann::json data;
        to_json(data, product);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
