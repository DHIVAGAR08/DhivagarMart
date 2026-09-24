#pragma once

#include <drogon/HttpController.h>
#include "service/ChatService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller handling customer inquiries with the AI shopping assistant.
 */
class ChatController : public drogon::HttpController<ChatController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ChatController::Chat, "/api/v1/ai/chat", drogon::Post, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter");
    METHOD_LIST_END

    ChatController();

    void Chat(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

private:
    std::shared_ptr<service::ChatService> chat_service_;
};

} // namespace dhivagar::dhivagarmart::controller
