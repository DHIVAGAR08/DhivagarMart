#include "plugin/MigrationPlugin.h"
#include "service/MigrationService.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::plugin {

void MigrationPlugin::initAndStart(const Json::Value &config) {
    try {
        spdlog::info("MigrationPlugin initializing database migrations...");
        service::MigrationService::RunPendingMigrations("db/migrations");
    } catch (const std::exception& e) {
        spdlog::error("MigrationPlugin failed during startup: {}", e.what());
    }
}

void MigrationPlugin::shutdown() {
    spdlog::info("MigrationPlugin shutdown.");
}

} // namespace dhivagar::dhivagarmart::plugin
