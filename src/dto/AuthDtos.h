#pragma once

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace dhivagar::dhivagarmart::dto {

struct RegisterRequestDto {
    std::string name;
    std::string email;
    std::string password;
    std::string role; // BUYER or SELLER only
};

inline void from_json(const nlohmann::json& j, RegisterRequestDto& dto) {
    dto.name = j.value("name", "");
    dto.email = j.value("email", "");
    dto.password = j.value("password", "");
    dto.role = j.value("role", "BUYER");
}

struct LoginRequestDto {
    std::string email;
    std::string password;
};

inline void from_json(const nlohmann::json& j, LoginRequestDto& dto) {
    dto.email = j.value("email", "");
    dto.password = j.value("password", "");
}

struct UserResponseDto {
    int64_t id{0};
    std::string name;
    std::string email;
    std::string role;
    std::string created_at;
};

inline void to_json(nlohmann::json& j, const UserResponseDto& dto) {
    j = nlohmann::json{
        {"id", dto.id},
        {"name", dto.name},
        {"email", dto.email},
        {"role", dto.role},
        {"created_at", dto.created_at}
    };
}

} // namespace dhivagar::dhivagarmart::dto
