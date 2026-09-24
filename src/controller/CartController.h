#pragma once

#include <cstdint>

#include <drogon/HttpController.h>
#include "service/CartService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller handling customer shopping cart management and running totals.
 */
class CartController : public drogon::HttpController<CartController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CartController::GetCart, "/api/v1/cart", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(CartController::AddItem, "/api/v1/cart", drogon::Post, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(CartController::UpdateQuantity, "/api/v1/cart/{1}", drogon::Put, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(CartController::RemoveItem, "/api/v1/cart/{1}", drogon::Delete, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    METHOD_LIST_END

    CartController();

    void GetCart(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void AddItem(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void UpdateQuantity(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);
    void RemoveItem(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);

private:
    std::shared_ptr<service::CartService> cart_service_;
};

} // namespace dhivagar::dhivagarmart::controller
