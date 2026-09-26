#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "model/WishlistItem.h"

namespace dhivagar::dhivagarmart::dto {

struct AddWishlistRequestDto {
    int64_t product_id{0};
};

inline void from_json(const nlohmann::json& j, AddWishlistRequestDto& dto) {
    dto.product_id = j.value("product_id", 0LL);
}

struct WishlistItemResponseDto {
    int64_t id{0};
    int64_t product_id{0};
    std::string product_name;
    std::string product_description;
    std::string product_category;
    std::string product_image_url;
    int64_t price_cents{0};
    std::string price_formatted;
    int32_t stock_qty{0};
    std::string created_at;

    static WishlistItemResponseDto FromModel(const model::WishlistItem& item) {
        WishlistItemResponseDto dto;
        dto.id = item.id;
        dto.product_id = item.product_id;
        dto.product_name = item.product_name;
        dto.product_description = item.product_description;
        dto.product_category = item.product_category;
        dto.product_image_url = item.product_image_url;
        dto.price_cents = item.product_price.GetCents();
        dto.price_formatted = item.product_price.ToString();
        dto.stock_qty = item.available_stock;
        dto.created_at = item.created_at;
        return dto;
    }
};

inline void to_json(nlohmann::json& j, const WishlistItemResponseDto& dto) {
    j = nlohmann::json{
        {"id", dto.id},
        {"product_id", dto.product_id},
        {"product_name", dto.product_name},
        {"product_description", dto.product_description},
        {"product_category", dto.product_category},
        {"product_image_url", dto.product_image_url},
        {"price_cents", dto.price_cents},
        {"price_formatted", dto.price_formatted},
        {"stock_qty", dto.stock_qty},
        {"created_at", dto.created_at}
    };
}

} // namespace dhivagar::dhivagarmart::dto
