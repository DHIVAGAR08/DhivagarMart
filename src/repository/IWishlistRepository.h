#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include "model/WishlistItem.h"

namespace dhivagar::dhivagarmart::repository {

class IWishlistRepository {
public:
    virtual ~IWishlistRepository() = default;

    virtual std::vector<model::WishlistItem> GetWishlistItems(int64_t user_id, const std::string& request_id = "") = 0;
    virtual bool AddItem(int64_t user_id, int64_t product_id, const std::string& request_id = "") = 0;
    virtual bool RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id = "") = 0;
    virtual bool IsInWishlist(int64_t user_id, int64_t product_id, const std::string& request_id = "") = 0;
};

} // namespace dhivagar::dhivagarmart::repository
