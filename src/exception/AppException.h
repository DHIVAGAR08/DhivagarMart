#pragma once

#include <stdexcept>
#include <string>

namespace dhivagar::dhivagarmart::exception {

class AppException : public std::runtime_error {
public:
    AppException(int http_status, std::string error_code, std::string message)
        : std::runtime_error(message),
          http_status_(http_status),
          error_code_(std::move(error_code)),
          user_message_(std::move(message)) {}

    [[nodiscard]] int GetHttpStatus() const noexcept { return http_status_; }
    [[nodiscard]] const std::string& GetErrorCode() const noexcept { return error_code_; }
    [[nodiscard]] const std::string& GetUserMessage() const noexcept { return user_message_; }

private:
    int http_status_;
    std::string error_code_;
    std::string user_message_;
};

class ValidationException : public AppException {
public:
    explicit ValidationException(std::string message)
        : AppException(400, "VALIDATION_ERROR", std::move(message)) {}
};

class AuthenticationException : public AppException {
public:
    explicit AuthenticationException(std::string message = "Invalid email or password")
        : AppException(401, "AUTHENTICATION_FAILED", std::move(message)) {}
};

class AuthorizationException : public AppException {
public:
    explicit AuthorizationException(std::string message = "You do not have permission to perform this action")
        : AppException(403, "FORBIDDEN", std::move(message)) {}
};

class NotFoundException : public AppException {
public:
    explicit NotFoundException(std::string message = "Requested resource not found")
        : AppException(404, "NOT_FOUND", std::move(message)) {}
};

class ConflictException : public AppException {
public:
    explicit ConflictException(std::string message = "Resource already exists")
        : AppException(409, "CONFLICT", std::move(message)) {}
};

class InternalServerException : public AppException {
public:
    explicit InternalServerException(std::string message = "An unexpected internal server error occurred")
        : AppException(500, "INTERNAL_SERVER_ERROR", std::move(message)) {}
};

} // namespace dhivagar::dhivagarmart::exception
