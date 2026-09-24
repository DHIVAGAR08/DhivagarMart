#include "exception/GlobalExceptionHandler.h"
#include "exception/AppException.h"
#include "util/JsonUtil.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::exception {

drogon::HttpResponsePtr GlobalExceptionHandler::HandleException(const std::exception& e, const std::string& request_id) {
    if (const auto* app_ex = dynamic_cast<const AppException*>(&e)) {
        spdlog::warn("[{}] AppException caught: status={} code={} msg={}", 
                     request_id, app_ex->GetHttpStatus(), app_ex->GetErrorCode(), app_ex->GetUserMessage());
        return util::JsonUtil::CreateErrorResponse(app_ex->GetHttpStatus(), app_ex->GetErrorCode(), app_ex->GetUserMessage());
    }

    spdlog::error("[{}] Unhandled std::exception: {}", request_id, e.what());
    // Never expose internal database messages or exception text to client
    return util::JsonUtil::CreateErrorResponse(500, "INTERNAL_SERVER_ERROR", "An unexpected error occurred. Please try again later.");
}

drogon::HttpResponsePtr GlobalExceptionHandler::HandleUnknownException(const std::string& request_id) {
    spdlog::error("[{}] Unknown exception caught", request_id);
    return util::JsonUtil::CreateErrorResponse(500, "INTERNAL_SERVER_ERROR", "An unexpected system error occurred.");
}

} // namespace dhivagar::dhivagarmart::exception
