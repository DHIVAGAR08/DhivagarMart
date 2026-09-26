#include <cstdint>
#include "service/WishlistService.h"
#include "exception/AppException.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

WishlistService::WishlistService(
    std::shared_ptr<repository::IWishlistRepository> wishlist_repo,
    std::shared_ptr<repository::ICartRepository> cart_repo,
    std::shared_ptr<repository::IProductRepository> product_repo)
    : wishlist_repo_(std::move(wishlist_repo)),
      cart_repo_(std::move(cart_repo)),
      product_repo_(std::move(product_repo)) {}

std::vector<dto::WishlistItemResponseDto> WishlistService::GetWishlist(int64_t user_id, const std::string& request_id) {
    auto items = wishlist_repo_->GetWishlistItems(user_id, request_id);
    std::vector<dto::WishlistItemResponseDto> dtos;
    dtos.reserve(items.size());
    for (const auto& item : items) {
        dtos.push_back(dto::WishlistItemResponseDto::FromModel(item));
    }
    return dtos;
}

void WishlistService::AddToWishlist(int64_t user_id, int64_t product_id, const std::string& request_id) {
    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }
    auto product = product_repo_->FindById(product_id, request_id);
    if (!product.has_value()) {
        throw exception::NotFoundException("Product not found");
    }
    wishlist_repo_->AddItem(user_id, product_id, request_id);
    spdlog::info("[{}] Product {} added to wishlist for user {}", request_id, product_id, user_id);
}

void WishlistService::RemoveFromWishlist(int64_t user_id, int64_t product_id, const std::string& request_id) {
    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }
    wishlist_repo_->RemoveItem(user_id, product_id, request_id);
    spdlog::info("[{}] Product {} removed from wishlist for user {}", request_id, product_id, user_id);
}

void WishlistService::MoveToCart(int64_t user_id, int64_t product_id, const std::string& request_id) {
    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }
    auto product = product_repo_->FindById(product_id, request_id);
    if (!product.has_value()) {
        throw exception::NotFoundException("Product not found");
    }
    if (product->stock_qty <= 0) {
        throw exception::ValidationException("Product is out of stock and cannot be moved to cart");
    }
    // Add to cart with quantity 1
    cart_repo_->AddOrIncrementItem(user_id, product_id, 1, request_id);
    // Remove from wishlist
    wishlist_repo_->RemoveItem(user_id, product_id, request_id);
    spdlog::info("[{}] Product {} moved from wishlist to cart for user {}", request_id, product_id, user_id);
}

} // namespace dhivagar::dhivagarmart::service
