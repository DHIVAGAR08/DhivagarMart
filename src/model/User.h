#pragma once

#include <string>
#include <cstdint>

namespace dhivagar::dhivagarmart::model {

enum class UserRole {
    kBuyer,
    kSeller,
    kAdmin
};

inline std::string UserRoleToString(UserRole role) {
    switch (role) {
        case UserRole::kBuyer: return "BUYER";
        case UserRole::kSeller: return "SELLER";
        case UserRole::kAdmin: return "ADMIN";
        default: return "BUYER";
    }
}

inline UserRole StringToUserRole(const std::string& role_str) {
    if (role_str == "ADMIN") return UserRole::kAdmin;
    if (role_str == "SELLER") return UserRole::kSeller;
    return UserRole::kBuyer;
}

struct User {
    int64_t id{0};
    std::string name;
    std::string email;
    std::string password_hash;
    UserRole role{UserRole::kBuyer};
    std::string created_at;
};

} // namespace dhivagar::dhivagarmart::model
