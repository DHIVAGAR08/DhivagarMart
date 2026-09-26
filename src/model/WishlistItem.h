#pragma once

#include <string>
#include <cstdint>
#include "model/Money.h"

namespace dhivagar::dhivagarmart::model {

struct WishlistItem {
    int64_t id{0};
    int64_t user_id{0};
    int64_t product_id{0};
    std::string product_name;
    std::string product_description;
    std::string product_category;
    std::string product_image_url;
    Money product_price{Money::FromCents(0)};
    int32_t available_stock{0};
    std::string created_at;
};

} // namespace dhivagar::dhivagarmart::model
