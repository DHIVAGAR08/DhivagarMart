#include <cstdint>
#include "controller/AuthController.h"
#include "repository/UserRepository.h"
#include "exception/GlobalExceptionHandler.h"
#include "util/JsonUtil.h"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::controller {

static std::string GetRequestId(const drogon::HttpRequestPtr &req) {
    if (req->getAttributes()->find("request_id")) {
        return req->getAttributes()->get<std::string>("request_id");
    }
    return "req-unknown";
}

AuthController::AuthController() {
    auto user_repo = std::make_shared<repository::UserRepository>();
    auth_service_ = std::make_shared<service::AuthService>(user_repo);
}

void AuthController::Register(const drogon::HttpRequestPtr &req, 
                             std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto json_body = nlohmann::json::parse(req->body());
        dto::RegisterRequestDto dto = json_body.get<dto::RegisterRequestDto>();

        auto user = auth_service_->Register(dto, request_id);

        nlohmann::json data;
        to_json(data, user);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k201Created));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AuthController::Login(const drogon::HttpRequestPtr &req, 
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto json_body = nlohmann::json::parse(req->body());
        dto::LoginRequestDto dto = json_body.get<dto::LoginRequestDto>();

        auto user = auth_service_->Login(dto, request_id);

        // Security Requirement: Discard pre-auth session and issue fresh session state
        auto session = req->session();
        if (session) {
            session->clear();
            session->insert("user_id", user.id);
            session->insert("role", user.role);
            session->insert("user_name", user.name);
            session->insert("user_email", user.email);
        }

        nlohmann::json data;
        to_json(data, user);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AuthController::Logout(const drogon::HttpRequestPtr &req, 
                           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto session = req->session();
        if (session) {
            session->clear();
        }
        callback(util::JsonUtil::CreateSuccessResponse(nlohmann::json{{"logged_out", true}}, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

void AuthController::GetCurrentUser(const drogon::HttpRequestPtr &req, 
                                   std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        auto session = req->session();
        if (!session || !session->find("user_id")) {
            callback(util::JsonUtil::CreateErrorResponse(
                401, "UNAUTHENTICATED", "No active session. Please log in."
            ));
            return;
        }

        int64_t user_id = session->get<int64_t>("user_id");
        auto user = auth_service_->GetUserById(user_id, request_id);

        nlohmann::json data;
        to_json(data, user);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
