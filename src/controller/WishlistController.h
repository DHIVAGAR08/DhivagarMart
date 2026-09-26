#pragma once

#include <cstdint>
#include <memory>
#include <drogon/HttpController.h>
#include "service/WishlistService.h"

namespace dhivagar::dhivagarmart::controller {

class WishlistController : public drogon::HttpController<WishlistController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(WishlistController::GetWishlist, "/api/v1/wishlist", drogon::Get,
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(WishlistController::AddItem, "/api/v1/wishlist", drogon::Post,
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(WishlistController::RemoveItem, "/api/v1/wishlist/{1}", drogon::Delete,
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    ADD_METHOD_TO(WishlistController::MoveToCart, "/api/v1/wishlist/{1}/move-to-cart", drogon::Post,
                  "dhivagar::dhivagarmart::filter::LoggingFilter", "dhivagar::dhivagarmart::filter::AuthFilter");
    METHOD_LIST_END

    WishlistController();

    void GetWishlist(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void AddItem(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void RemoveItem(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);
    void MoveToCart(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t product_id);

private:
    std::shared_ptr<service::WishlistService> wishlist_service_;
};

} // namespace dhivagar::dhivagarmart::controller
