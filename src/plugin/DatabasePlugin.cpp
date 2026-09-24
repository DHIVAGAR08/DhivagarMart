#include "plugin/DatabasePlugin.h"
#include "util/Environment.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include <chrono>

namespace dhivagar::dhivagarmart::plugin {

DatabasePlugin* DatabasePlugin::instance_ = nullptr;

DatabasePlugin::DatabasePlugin() {
    instance_ = this;
}

DatabasePlugin::~DatabasePlugin() {
    shutdown();
    if (instance_ == this) {
        instance_ = nullptr;
    }
}

std::string DatabasePlugin::BuildConnectionString() {
    util::Environment::LoadEnvFile(".env");
    std::string host = util::Environment::Get("DATABASE_HOST", "127.0.0.1");
    std::string port = util::Environment::Get("DATABASE_PORT", "5433");
    std::string dbname = util::Environment::Get("DATABASE_NAME", "dhivagarmart");
    std::string user = util::Environment::Get("DATABASE_USER", "postgres");
    std::string password = util::Environment::Get("DATABASE_PASSWORD", "");

    std::string conn_str = "host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user;
    if (!password.empty()) {
        conn_str += " password=" + password;
    }
    conn_str += " connect_timeout=5";
    return conn_str;
}

void DatabasePlugin::initAndStart(const Json::Value &config) {
    if (is_running_) {
        return;
    }

    connection_string_ = BuildConnectionString();

    try {
        // Test primary connection
        auto primary_conn = std::make_shared<pqxx::connection>(connection_string_);
        if (!primary_conn->is_open()) {
            throw std::runtime_error("Database connection opened in invalid state");
        }

        {
            pqxx::work tx(*primary_conn);
            pqxx::result r = tx.exec("SELECT 1;");
            tx.commit();
        }

        // Required exact specification log output
        std::cout << "PostgreSQL connected successfully" << std::endl;
        spdlog::info("PostgreSQL connected successfully");

        {
            std::lock_guard<std::mutex> lock(mutex_);
            pool_.push(primary_conn);

            // Pre-warm the pool to min_connections_
            for (size_t i = 1; i < min_connections_; ++i) {
                try {
                    auto c = std::make_shared<pqxx::connection>(connection_string_);
                    if (c->is_open()) {
                        pool_.push(c);
                    }
                } catch (...) {
                    break;
                }
            }
            is_running_ = true;
        }
    } catch (const std::exception& e) {
        // Safe user-readable error without exposing database passwords
        std::string safe_msg = "Database connection failed: unable to connect to host/port specified.";
        std::cerr << safe_msg << " Details: " << e.what() << std::endl;
        spdlog::error("{}. Error: {}", safe_msg, e.what());
        is_running_ = false;
        throw std::runtime_error(safe_msg);
    }
}

void DatabasePlugin::shutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    is_running_ = false;
    while (!pool_.empty()) {
        auto conn = pool_.front();
        pool_.pop();
        try {
            if (conn && conn->is_open()) {
                conn->close();
            }
        } catch (...) {}
    }
    cv_.notify_all();
    spdlog::info("DatabasePlugin shutdown complete");
}

void DatabasePlugin::InitAndStart() {
    if (!instance_) {
        static DatabasePlugin s_plugin;
        instance_ = &s_plugin;
    }
    Json::Value empty_cfg;
    instance_->initAndStart(empty_cfg);
}

void DatabasePlugin::Shutdown() {
    if (instance_) {
        instance_->shutdown();
    }
}

std::shared_ptr<pqxx::connection> DatabasePlugin::AcquireInternal() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (!is_running_) {
        // Attempt on-demand initialization if not yet started
        lock.unlock();
        InitAndStart();
        lock.lock();
    }

    if (cv_.wait_for(lock, std::chrono::seconds(5), [this]() { return !pool_.empty() || !is_running_; })) {
        if (!is_running_) {
            throw std::runtime_error("Database connection pool is shutting down");
        }
        auto conn = pool_.front();
        pool_.pop();

        // Verify connection is healthy
        if (!conn || !conn->is_open()) {
            conn = std::make_shared<pqxx::connection>(connection_string_);
        }
        return conn;
    }

    // Try creating an ad-hoc connection if pool is starved
    return std::make_shared<pqxx::connection>(connection_string_);
}

void DatabasePlugin::ReleaseInternal(std::shared_ptr<pqxx::connection> conn) {
    if (!conn) return;
    std::lock_guard<std::mutex> lock(mutex_);
    if (is_running_ && conn->is_open() && pool_.size() < max_connections_) {
        pool_.push(std::move(conn));
        cv_.notify_one();
    } else {
        try {
            conn->close();
        } catch (...) {}
    }
}

ConnectionHandle DatabasePlugin::GetConnection() {
    if (!instance_) {
        InitAndStart();
    }
    auto conn = instance_->AcquireInternal();
    return ConnectionHandle(conn, [](std::shared_ptr<pqxx::connection> c) {
        if (instance_) {
            instance_->ReleaseInternal(std::move(c));
        }
    });
}

bool DatabasePlugin::CheckHealth() {
    try {
        auto handle = GetConnection();
        pqxx::nontransaction ntx(*handle);
        pqxx::result r = ntx.exec("SELECT 1;");
        return !r.empty();
    } catch (const std::exception& e) {
        spdlog::warn("Database health check failed: {}", e.what());
        return false;
    }
}

} // namespace dhivagar::dhivagarmart::plugin
