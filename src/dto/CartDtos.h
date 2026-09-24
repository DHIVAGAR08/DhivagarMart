#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "model/CartItem.h"

namespace dhivagar::dhivagarmart::dto {

struct AddCartItemRequestDto {
    int64_t product_id{0};
    int32_t quantity{1};
};

inline void from_json(const nlohmann::json& j, AddCartItemRequestDto& dto) {
    dto.product_id = j.value("product_id", 0LL);
    dto.quantity = j.value("quantity", 1);
}

struct UpdateCartItemRequestDto {
    int32_t quantity{1};
};

inline void from_json(const nlohmann::json& j, UpdateCartItemRequestDto& dto) {
    dto.quantity = j.value("quantity", 1);
}

struct CartItemResponseDto {
    int64_t id{0};
    int64_t product_id{0};
    std::string product_name;
    std::string product_category;
    std::string product_image_url;
    int64_t unit_price_cents{0};
    std::string unit_price_formatted;
    int32_t available_stock{0};
    int32_t quantity{0};
    int64_t subtotal_cents{0};
    std::string subtotal_formatted;

    static CartItemResponseDto FromModel(const model::CartItem& item) {
        CartItemResponseDto dto;
        dto.id = item.id;
        dto.product_id = item.product_id;
        dto.product_name = item.product_name;
        dto.product_category = item.product_category;
        dto.product_image_url = item.product_image_url;
        dto.unit_price_cents = item.product_price.GetCents();
        dto.unit_price_formatted = item.product_price.ToString();
        dto.available_stock = item.available_stock;
        dto.quantity = item.quantity;
        dto.subtotal_cents = item.GetSubtotal().GetCents();
        dto.subtotal_formatted = item.GetSubtotal().ToString();
        return dto;
    }
};

inline void to_json(nlohmann::json& j, const CartItemResponseDto& dto) {
    j = nlohmann::json{
        {"id", dto.id},
        {"product_id", dto.product_id},
        {"product_name", dto.product_name},
        {"product_category", dto.product_category},
        {"product_image_url", dto.product_image_url},
        {"unit_price_cents", dto.unit_price_cents},
        {"unit_price_formatted", dto.unit_price_formatted},
        {"available_stock", dto.available_stock},
        {"quantity", dto.quantity},
        {"subtotal_cents", dto.subtotal_cents},
        {"subtotal_formatted", dto.subtotal_formatted}
    };
}

struct CartResponseDto {
    std::vector<CartItemResponseDto> items;
    int32_t total_items{0};
    int64_t total_cents{0};
    std::string total_formatted;
};

inline void to_json(nlohmann::json& j, const CartResponseDto& dto) {
    j = nlohmann::json{
        {"items", dto.items},
        {"total_items", dto.total_items},
        {"total_cents", dto.total_cents},
        {"total_formatted", dto.total_formatted}
    };
}

} // namespace dhivagar::dhivagarmart::dto
