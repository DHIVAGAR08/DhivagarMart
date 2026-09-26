#include <cstdint>
#include "controller/WishlistController.h"
#include "repository/WishlistRepository.h"
#include "repository/CartRepository.h"
#include "repository/ProductRepository.h"
#include "util/JsonUtil.h"
#include "exception/GlobalExceptionHandler.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::controller {

WishlistController::WishlistController() {
    auto wishlist_repo = std::make_shared<repository::WishlistRepository>();
    auto cart_repo = std::make_shared<repository::CartRepository>();
    auto product_repo = std::make_shared<repository::ProductRepository>();
    wishlist_service_ = std::make_shared<service::WishlistService>(wishlist_repo, cart_repo, product_repo);
}

void WishlistController::GetWishlist(
    const drogon::HttpRequestPtr &req, 
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string req_id = req->getHeader("X-Request-ID");
    try {
        auto session = req->session();
        int64_t user_id = session->get<int64_t>("user_id");

        auto items = wishlist_service_->GetWishlist(user_id, req_id);
        nlohmann::json j_items = items;
        callback(util::JsonUtil::CreateSuccessResponse(j_items));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, req_id));
    } catch (...) {
        callback(exception::GlobalExceptionHandler::HandleUnknownException(req_id));
    }
}

void WishlistController::AddItem(
    const drogon::HttpRequestPtr &req, 
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string req_id = req->getHeader("X-Request-ID");
    try {
        auto session = req->session();
        int64_t user_id = session->get<int64_t>("user_id");

        auto body = nlohmann::json::parse(req->body());
        dto::AddWishlistRequestDto dto;
        dto::from_json(body, dto);

        wishlist_service_->AddToWishlist(user_id, dto.product_id, req_id);
        callback(util::JsonUtil::CreateSuccessResponse(
            nlohmann::json{{"message", "Product added to wishlist successfully"}}, 
            drogon::k201Created
        ));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, req_id));
    } catch (...) {
        callback(exception::GlobalExceptionHandler::HandleUnknownException(req_id));
    }
}

void WishlistController::RemoveItem(
    const drogon::HttpRequestPtr &req, 
    std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
    int64_t product_id) {
    std::string req_id = req->getHeader("X-Request-ID");
    try {
        auto session = req->session();
        int64_t user_id = session->get<int64_t>("user_id");

        wishlist_service_->RemoveFromWishlist(user_id, product_id, req_id);
        callback(util::JsonUtil::CreateSuccessResponse(
            nlohmann::json{{"message", "Item removed from wishlist successfully"}}
        ));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, req_id));
    } catch (...) {
        callback(exception::GlobalExceptionHandler::HandleUnknownException(req_id));
    }
}

void WishlistController::MoveToCart(
    const drogon::HttpRequestPtr &req, 
    std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
    int64_t product_id) {
    std::string req_id = req->getHeader("X-Request-ID");
    try {
        auto session = req->session();
        int64_t user_id = session->get<int64_t>("user_id");

        wishlist_service_->MoveToCart(user_id, product_id, req_id);
        callback(util::JsonUtil::CreateSuccessResponse(
            nlohmann::json{{"message", "Product moved to shopping cart successfully"}}
        ));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, req_id));
    } catch (...) {
        callback(exception::GlobalExceptionHandler::HandleUnknownException(req_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
