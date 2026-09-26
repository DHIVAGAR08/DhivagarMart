#pragma once

#include <cstdint>

#include <drogon/HttpController.h>
#include "service/SellerService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller handling seller product inventory and incoming fulfillment orders.
 */
class SellerController : public drogon::HttpController<SellerController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SellerController::GetProducts, "/api/v1/seller/products", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::SellerFilter");
    ADD_METHOD_TO(SellerController::CreateProduct, "/api/v1/seller/products", drogon::Post, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::SellerFilter");
    ADD_METHOD_TO(SellerController::UpdateProduct, "/api/v1/seller/products/{1}", drogon::Put, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::SellerFilter");
    ADD_METHOD_TO(SellerController::DeleteProduct, "/api/v1/seller/products/{1}", drogon::Delete, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::SellerFilter");
    ADD_METHOD_TO(SellerController::GetOrders, "/api/v1/seller/orders", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::SellerFilter");
    ADD_METHOD_TO(SellerController::UpdateOrderStatus, "/api/v1/seller/orders/{1}/status", drogon::Put, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::SellerFilter");
    ADD_METHOD_TO(SellerController::GetStats, "/api/v1/seller/stats", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::SellerFilter");
    METHOD_LIST_END

    SellerController();

    void GetStats(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void GetProducts(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void CreateProduct(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void UpdateProduct(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);
    void DeleteProduct(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);
    void GetOrders(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void UpdateOrderStatus(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t order_id);

private:
    std::shared_ptr<service::SellerService> seller_service_;
};

} // namespace dhivagar::dhivagarmart::controller
