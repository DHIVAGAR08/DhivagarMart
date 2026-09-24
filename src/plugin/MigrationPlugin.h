#pragma once

#include <drogon/plugins/Plugin.h>

namespace dhivagar::dhivagarmart::plugin {

/**
 * @brief Startup Drogon Plugin that invokes MigrationService to automatically apply schema migrations.
 */
class MigrationPlugin : public drogon::Plugin<MigrationPlugin> {
public:
    MigrationPlugin() = default;
    ~MigrationPlugin() override = default;

    void initAndStart(const Json::Value &config) override;
    void shutdown() override;
};

} // namespace dhivagar::dhivagarmart::plugin
