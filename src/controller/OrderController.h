#pragma once

#include <cstdint>

#include <drogon/HttpController.h>
#include "service/OrderService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller managing order placement, transactional checkout, and buyer order histories.
 */
class OrderController : public drogon::HttpController<OrderController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(OrderController::Checkout, "/api/v1/orders", drogon::Post, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(OrderController::GetOrders, "/api/v1/orders", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(OrderController::GetOrderById, "/api/v1/orders/{1}", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    METHOD_LIST_END

    OrderController();

    void Checkout(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void GetOrders(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void GetOrderById(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t order_id);

private:
    std::shared_ptr<service::OrderService> order_service_;
};

} // namespace dhivagar::dhivagarmart::controller
