#include "controller/ChatController.h"
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

ChatController::ChatController() {
    chat_service_ = std::make_shared<service::ChatService>();
}

void ChatController::Chat(const drogon::HttpRequestPtr &req, 
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::string request_id = GetRequestId(req);
    try {
        std::string session_id = "anon-session";
        auto session = req->session();
        if (session) {
            session_id = session->sessionId();
        }

        auto json_body = nlohmann::json::parse(req->body());
        dto::ChatRequestDto dto = json_body.get<dto::ChatRequestDto>();

        auto reply_dto = chat_service_->Chat(session_id, dto.message, request_id);

        nlohmann::json data;
        to_json(data, reply_dto);
        callback(util::JsonUtil::CreateSuccessResponse(data, drogon::k200OK));
    } catch (const std::exception& e) {
        callback(exception::GlobalExceptionHandler::HandleException(e, request_id));
    }
}

} // namespace dhivagar::dhivagarmart::controller
