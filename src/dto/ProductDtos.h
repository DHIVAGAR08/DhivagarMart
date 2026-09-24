#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "model/Product.h"

namespace dhivagar::dhivagarmart::dto {

struct ProductRequestDto {
    std::string name;
    std::string description;
    int64_t price_cents{0};
    int32_t stock_qty{0};
    std::string category;
    std::string image_url;
};

inline void from_json(const nlohmann::json& j, ProductRequestDto& dto) {
    dto.name = j.value("name", "");
    dto.description = j.value("description", "");
    if (j.contains("price_cents")) {
        dto.price_cents = j.at("price_cents").get<int64_t>();
    } else if (j.contains("price")) {
        // Support either decimal or cents gracefully
        if (j["price"].is_number_float()) {
            dto.price_cents = static_cast<int64_t>(j["price"].get<double>() * 100.0 + 0.5);
        } else {
            dto.price_cents = j["price"].get<int64_t>();
        }
    }
    dto.stock_qty = j.value("stock_qty", 0);
    dto.category = j.value("category", "");
    dto.image_url = j.value("image_url", "");
}

struct ProductResponseDto {
    int64_t id{0};
    int64_t seller_id{0};
    std::string name;
    std::string description;
    int64_t price_cents{0};
    std::string price_formatted;
    int32_t stock_qty{0};
    std::string category;
    std::string image_url;
    std::string created_at;

    static ProductResponseDto FromModel(const model::Product& product) {
        ProductResponseDto dto;
        dto.id = product.id;
        dto.seller_id = product.seller_id;
        dto.name = product.name;
        dto.description = product.description;
        dto.price_cents = product.price.GetCents();
        dto.price_formatted = product.price.ToString();
        dto.stock_qty = product.stock_qty;
        dto.category = product.category;
        dto.image_url = product.image_url;
        dto.created_at = product.created_at;
        return dto;
    }
};

inline void to_json(nlohmann::json& j, const ProductResponseDto& dto) {
    j = nlohmann::json{
        {"id", dto.id},
        {"seller_id", dto.seller_id},
        {"name", dto.name},
        {"description", dto.description},
        {"price_cents", dto.price_cents},
        {"price_formatted", dto.price_formatted},
        {"stock_qty", dto.stock_qty},
        {"category", dto.category},
        {"image_url", dto.image_url},
        {"created_at", dto.created_at}
    };
}

} // namespace dhivagar::dhivagarmart::dto
