#pragma once

#include <cstdint>

#include <drogon/HttpController.h>
#include "service/AdminService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller handling administrative moderation, user management, and business analytics.
 */
class AdminController : public drogon::HttpController<AdminController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AdminController::GetAllUsers, "/api/v1/admin/users", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AdminFilter");
    ADD_METHOD_TO(AdminController::GetAllProducts, "/api/v1/admin/products", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AdminFilter");
    ADD_METHOD_TO(AdminController::ModerateDeleteProduct, "/api/v1/admin/products/{1}", drogon::Delete, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AdminFilter");
    ADD_METHOD_TO(AdminController::GetAllOrders, "/api/v1/admin/orders", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AdminFilter");
    ADD_METHOD_TO(AdminController::UpdateOrderStatus, "/api/v1/admin/orders/{1}/status", drogon::Put, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AdminFilter");
    ADD_METHOD_TO(AdminController::GetStatistics, "/api/v1/admin/stats", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AdminFilter");
    METHOD_LIST_END

    AdminController();

    void GetAllUsers(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void GetAllProducts(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void ModerateDeleteProduct(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);
    void GetAllOrders(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void UpdateOrderStatus(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t order_id);
    void GetStatistics(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

private:
    std::shared_ptr<service::AdminService> admin_service_;
};

} // namespace dhivagar::dhivagarmart::controller
