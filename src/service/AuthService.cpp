#include <cstdint>
#include "service/AuthService.h"
#include "util/PasswordUtil.h"
#include "util/ValidationUtil.h"
#include "exception/AppException.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

AuthService::AuthService(std::shared_ptr<repository::IUserRepository> user_repo)
    : user_repo_(std::move(user_repo)) {}

dto::UserResponseDto AuthService::Register(const dto::RegisterRequestDto& req, const std::string& request_id) {
    spdlog::info("[{}] AuthService::Register attempt for email: {}", request_id, req.email);

    // 1. Validation at the top of the service method
    std::string trimmed_name = util::ValidationUtil::Trim(req.name);
    std::string trimmed_email = util::ValidationUtil::Trim(req.email);

    if (trimmed_name.empty()) {
        throw exception::ValidationException("Name cannot be empty");
    }
    if (!util::ValidationUtil::IsValidEmail(trimmed_email)) {
        throw exception::ValidationException("Invalid email address format");
    }
    if (!util::ValidationUtil::IsValidPassword(req.password)) {
        throw exception::ValidationException("Password must be at least 6 characters long");
    }
    if (!util::ValidationUtil::IsValidRole(req.role)) {
        throw exception::ValidationException("Registration role must be either BUYER or SELLER");
    }

    // 2. Uniqueness check
    if (user_repo_->FindByEmail(trimmed_email, request_id).has_value()) {
        throw exception::ConflictException("An account with this email address already exists");
    }

    // 3. Argon2id password hashing
    std::string hashed_pw = util::PasswordUtil::HashPassword(req.password);

    // 4. Persistence
    model::User user;
    user.name = trimmed_name;
    user.email = trimmed_email;
    user.password_hash = hashed_pw;
    user.role = model::StringToUserRole(req.role);

    auto created_user = user_repo_->Create(user, request_id);

    spdlog::info("[{}] AuthService::Register success, user ID: {}", request_id, created_user.id);

    return dto::UserResponseDto{
        .id = created_user.id,
        .name = created_user.name,
        .email = created_user.email,
        .role = model::UserRoleToString(created_user.role),
        .created_at = created_user.created_at
    };
}

dto::UserResponseDto AuthService::Login(const dto::LoginRequestDto& req, const std::string& request_id) {
    spdlog::info("[{}] AuthService::Login attempt for email: {}", request_id, req.email);

    std::string trimmed_email = util::ValidationUtil::Trim(req.email);
    if (!util::ValidationUtil::IsValidEmail(trimmed_email) || req.password.empty()) {
        throw exception::AuthenticationException("Invalid email or password");
    }

    auto user_opt = user_repo_->FindByEmail(trimmed_email, request_id);
    if (!user_opt.has_value()) {
        // Generic failure message to prevent user enumeration
        throw exception::AuthenticationException("Invalid email or password");
    }

    const auto& user = *user_opt;
    if (!util::PasswordUtil::VerifyPassword(user.password_hash, req.password)) {
        spdlog::warn("[{}] AuthService::Login password mismatch for user: {}", request_id, user.id);
        throw exception::AuthenticationException("Invalid email or password");
    }

    spdlog::info("[{}] AuthService::Login success for user ID: {}", request_id, user.id);

    return dto::UserResponseDto{
        .id = user.id,
        .name = user.name,
        .email = user.email,
        .role = model::UserRoleToString(user.role),
        .created_at = user.created_at
    };
}

dto::UserResponseDto AuthService::GetUserById(int64_t user_id, const std::string& request_id) {
    auto user_opt = user_repo_->FindById(user_id, request_id);
    if (!user_opt.has_value()) {
        throw exception::NotFoundException("User not found");
    }

    const auto& user = *user_opt;
    return dto::UserResponseDto{
        .id = user.id,
        .name = user.name,
        .email = user.email,
        .role = model::UserRoleToString(user.role),
        .created_at = user.created_at
    };
}

} // namespace dhivagar::dhivagarmart::service
