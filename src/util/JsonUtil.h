#pragma once

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include <drogon/HttpResponse.h>

namespace dhivagar::dhivagarmart::util {

class JsonUtil {
public:
    static nlohmann::json SuccessEnvelope(const nlohmann::json& data) {
        return nlohmann::json{
            {"success", true},
            {"data", data},
            {"error", nullptr}
        };
    }

    static nlohmann::json ErrorEnvelope(const std::string& code, const std::string& message) {
        return nlohmann::json{
            {"success", false},
            {"data", nullptr},
            {"error", {
                {"code", code},
                {"message", message}
            }}
        };
    }

    static drogon::HttpResponsePtr CreateSuccessResponse(const nlohmann::json& data, drogon::HttpStatusCode status = drogon::k200OK) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(status);
        resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
        resp->setBody(SuccessEnvelope(data).dump());
        return resp;
    }

    static drogon::HttpResponsePtr CreateErrorResponse(int status_code, const std::string& error_code, const std::string& message) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(static_cast<drogon::HttpStatusCode>(status_code));
        resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
        resp->setBody(ErrorEnvelope(error_code, message).dump());
        return resp;
    }
};

} // namespace dhivagar::dhivagarmart::util
