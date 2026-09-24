#include <drogon/drogon.h>
#include "util/Environment.h"
#include "plugin/DatabasePlugin.h"
#include "plugin/MigrationPlugin.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <iostream>
#include <vector>

using namespace dhivagar::dhivagarmart;

int main() {
    try {
        // 1. Initialize structured spdlog loggers (console + rotating file)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/dhivagarmart.log", 1024 * 1024 * 10, 3);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("dhivagarmart", sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::debug);
        spdlog::set_default_logger(logger);
        spdlog::flush_on(spdlog::level::info);

        // 2. Load Environment Variables
        util::Environment::LoadEnvFile(".env");
        std::string host = util::Environment::Get("APP_HOST", "0.0.0.0");
        int port = util::Environment::GetInt("APP_PORT", 8080);
        int session_timeout = util::Environment::GetInt("SESSION_IDLE_TIMEOUT_SEC", 1800);

        std::cout << R"(
============================================================
           🛒 DHIVAGAR MART - NATIVE C++20 BACKEND
============================================================
  Web Framework:       Drogon HTTP Server
  Language Standard:   ISO C++20
  Database:            PostgreSQL 15+ (libpqxx)
  Security Engine:     libsodium Argon2id
  Listening Address:   http://)" << host << ":" << port << R"(
  Document Root:       ./frontend
  Session Timeout:     )" << session_timeout << R"( seconds
============================================================
)" << std::endl;

        // 3. Configure Drogon Application
        drogon::app()
            .addListener(host, port)
            .setDocumentRoot("frontend")
            .enableSession(session_timeout)
            .setServerHeaderField("DhivagarMart/1.0.0");

        // Map root / to index.html
        drogon::app().registerHandler(
            "/",
            [](const drogon::HttpRequestPtr &,
               std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
                auto resp = drogon::HttpResponse::newHttpViewResponse("index.html");
                callback(resp);
            },
            {drogon::Get}
        );

        // 4. Initialize Database Connection Pool and Run Migrations
        plugin::DatabasePlugin::InitAndStart();
        plugin::MigrationPlugin mig_plugin;
        Json::Value empty_cfg;
        mig_plugin.initAndStart(empty_cfg);

        // 5. Run Server Event Loop
        spdlog::info("Dhivagar Mart server listening on {}:{}", host, port);
        drogon::app().run();

        // 6. Graceful Shutdown
        plugin::DatabasePlugin::Shutdown();
        spdlog::info("Dhivagar Mart server shut down cleanly.");

    } catch (const std::exception& e) {
        std::cerr << "Fatal error during startup: " << e.what() << std::endl;
        spdlog::critical("Fatal application startup failure: {}", e.what());
        return 1;
    }

    return 0;
}
