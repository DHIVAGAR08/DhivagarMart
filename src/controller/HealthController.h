#pragma once

#include <drogon/HttpController.h>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller providing liveness and database connectivity health probe.
 */
class HealthController : public drogon::HttpController<HealthController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(HealthController::CheckHealth, "/api/v1/health", drogon::Get, 
                  "dhivagar::dhivagarmart::filter::LoggingFilter");
    METHOD_LIST_END

    void CheckHealth(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

} // namespace dhivagar::dhivagarmart::controller
