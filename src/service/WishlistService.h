#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include "repository/IWishlistRepository.h"
#include "repository/ICartRepository.h"
#include "repository/IProductRepository.h"
#include "dto/WishlistDtos.h"

namespace dhivagar::dhivagarmart::service {

class WishlistService {
public:
    WishlistService(std::shared_ptr<repository::IWishlistRepository> wishlist_repo,
                    std::shared_ptr<repository::ICartRepository> cart_repo,
                    std::shared_ptr<repository::IProductRepository> product_repo);

    std::vector<dto::WishlistItemResponseDto> GetWishlist(int64_t user_id, const std::string& request_id = "");
    void AddToWishlist(int64_t user_id, int64_t product_id, const std::string& request_id = "");
    void RemoveFromWishlist(int64_t user_id, int64_t product_id, const std::string& request_id = "");
    void MoveToCart(int64_t user_id, int64_t product_id, const std::string& request_id = "");

private:
    std::shared_ptr<repository::IWishlistRepository> wishlist_repo_;
    std::shared_ptr<repository::ICartRepository> cart_repo_;
    std::shared_ptr<repository::IProductRepository> product_repo_;
};

} // namespace dhivagar::dhivagarmart::service
