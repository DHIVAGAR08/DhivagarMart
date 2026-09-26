#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "repository/IWishlistRepository.h"

namespace dhivagar::dhivagarmart::repository {

class WishlistRepository : public IWishlistRepository {
public:
    WishlistRepository() = default;
    ~WishlistRepository() override = default;

    std::vector<model::WishlistItem> GetWishlistItems(int64_t user_id, const std::string& request_id = "") override;
    bool AddItem(int64_t user_id, int64_t product_id, const std::string& request_id = "") override;
    bool RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id = "") override;
    bool IsInWishlist(int64_t user_id, int64_t product_id, const std::string& request_id = "") override;
};

} // namespace dhivagar::dhivagarmart::repository
