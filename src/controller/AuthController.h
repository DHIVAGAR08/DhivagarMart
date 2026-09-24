#pragma once

#include <drogon/HttpController.h>
#include "service/AuthService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller handling customer registration, credential verification, and session control.
 */
class AuthController : public drogon::HttpController<AuthController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AuthController::Register, "/api/v1/auth/register", drogon::Post, "dhivagar::dhivagarmart::filter::LoggingFilter");
    ADD_METHOD_TO(AuthController::Login, "/api/v1/auth/login", drogon::Post, "dhivagar::dhivagarmart::filter::LoggingFilter");
    ADD_METHOD_TO(AuthController::Logout, "/api/v1/auth/logout", drogon::Post, "dhivagar::dhivagarmart::filter::LoggingFilter");
    ADD_METHOD_TO(AuthController::GetCurrentUser, "/api/v1/auth/me", drogon::Get, "dhivagar::dhivagarmart::filter::LoggingFilter");
    METHOD_LIST_END

    AuthController();

    void Register(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void Login(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void Logout(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void GetCurrentUser(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

private:
    std::shared_ptr<service::AuthService> auth_service_;
};

} // namespace dhivagar::dhivagarmart::controller
