#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "model/Review.h"

namespace dhivagar::dhivagarmart::dto {

struct CreateReviewRequestDto {
    int32_t rating{5};
    std::string comment;
};

inline void from_json(const nlohmann::json& j, CreateReviewRequestDto& dto) {
    dto.rating = j.value("rating", 5);
    dto.comment = j.value("comment", "");
}

struct ReviewResponseDto {
    int64_t id{0};
    int64_t product_id{0};
    int64_t user_id{0};
    std::string user_name;
    int32_t rating{5};
    std::string comment;
    std::string created_at;

    static ReviewResponseDto FromModel(const model::Review& review) {
        ReviewResponseDto dto;
        dto.id = review.id;
        dto.product_id = review.product_id;
        dto.user_id = review.user_id;
        dto.user_name = review.user_name;
        dto.rating = review.rating;
        dto.comment = review.comment;
        dto.created_at = review.created_at;
        return dto;
    }
};

inline void to_json(nlohmann::json& j, const ReviewResponseDto& dto) {
    j = nlohmann::json{
        {"id", dto.id},
        {"product_id", dto.product_id},
        {"user_id", dto.user_id},
        {"user_name", dto.user_name},
        {"rating", dto.rating},
        {"comment", dto.comment},
        {"created_at", dto.created_at}
    };
}

} // namespace dhivagar::dhivagarmart::dto
