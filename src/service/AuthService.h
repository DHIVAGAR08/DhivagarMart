#pragma once

#include <cstdint>

#include "repository/IUserRepository.h"
#include "dto/AuthDtos.h"
#include <memory>
#include <string>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Service responsible for user authentication and account registration.
 */
class AuthService {
public:
    /**
     * @brief Constructor utilizing Dependency Injection.
     * @param user_repo Injected user repository interface.
     */
    explicit AuthService(std::shared_ptr<repository::IUserRepository> user_repo);

    /**
     * @brief Registers a new user account (BUYER or SELLER role only).
     * @param req Registration payload containing name, email, password, and role.
     * @param request_id Correlation ID for structured request logging.
     * @return User response DTO excluding sensitive password data.
     * @throws ValidationException on invalid input.
     * @throws ConflictException if email is already registered.
     */
    dto::UserResponseDto Register(const dto::RegisterRequestDto& req, const std::string& request_id = "");

    /**
     * @brief Authenticates user credentials.
     * @param req Login payload containing email and plaintext password.
     * @param request_id Correlation ID.
     * @return User response DTO on successful verification.
     * @throws AuthenticationException on invalid credentials.
     */
    dto::UserResponseDto Login(const dto::LoginRequestDto& req, const std::string& request_id = "");

    /**
     * @brief Fetches current authenticated user profile by ID.
     * @param user_id User ID.
     * @param request_id Correlation ID.
     * @return User profile DTO.
     * @throws NotFoundException if user is not found.
     */
    dto::UserResponseDto GetUserById(int64_t user_id, const std::string& request_id = "");

private:
    std::shared_ptr<repository::IUserRepository> user_repo_;
};

} // namespace dhivagar::dhivagarmart::service
