#include "controller/HealthController.h"
#include "plugin/DatabasePlugin.h"
#include "util/JsonUtil.h"
#include <nlohmann/json.hpp>

namespace dhivagar::dhivagarmart::controller {

void HealthController::CheckHealth(const drogon::HttpRequestPtr &/*req*/, 
                                   std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    bool db_ok = plugin::DatabasePlugin::CheckHealth();

    nlohmann::json data = {
        {"status", "UP"},
        {"db", db_ok ? "UP" : "DOWN"}
    };

    nlohmann::json response_body = {
        {"success", true},
        {"data", data},
        {"status", "UP"},
        {"db", db_ok ? "UP" : "DOWN"}
    };

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k200OK);
    resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    resp->setBody(response_body.dump());
    callback(resp);
}

} // namespace dhivagar::dhivagarmart::controller
