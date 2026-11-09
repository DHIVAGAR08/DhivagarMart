#include "plugin/DatabasePlugin.h"
#include "util/Environment.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include <chrono>
#include <thread>

namespace dhivagar::dhivagarmart::plugin {

DatabasePlugin* DatabasePlugin::instance_ = nullptr;
std::string DatabasePlugin::last_error_ = "Not initialized";
std::string DatabasePlugin::database_host_ = "unknown";

std::string DatabasePlugin::GetLastError() {
    return last_error_;
}

std::string DatabasePlugin::GetDatabaseHost() {
    return database_host_;
}

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
    std::string db_url = util::Environment::Get("DATABASE_URL", "");
    if (!db_url.empty()) {
        auto at_pos = db_url.find('@');
        if (at_pos != std::string::npos) {
            auto colon_pos = db_url.find(':', at_pos + 1);
            auto slash_pos = db_url.find('/', at_pos + 1);
            auto end_pos = (colon_pos != std::string::npos) ? colon_pos : slash_pos;
            if (end_pos != std::string::npos && end_pos > at_pos + 1) {
                database_host_ = db_url.substr(at_pos + 1, end_pos - (at_pos + 1));
            } else {
                database_host_ = db_url.substr(at_pos + 1);
            }
        }
        if (db_url.find("connect_timeout") == std::string::npos) {
            if (db_url.find('?') == std::string::npos) {
                db_url += "?connect_timeout=5";
            } else {
                db_url += "&connect_timeout=5";
            }
        }
        return db_url;
    }
    std::string host = util::Environment::Get("DATABASE_HOST", util::Environment::Get("DB_HOST", "127.0.0.1"));
    database_host_ = host;
    std::string port = util::Environment::Get("DATABASE_PORT", util::Environment::Get("DB_PORT", "5432"));
    std::string dbname = util::Environment::Get("DATABASE_NAME", util::Environment::Get("DB_NAME", "dhivagarmart"));
    std::string user = util::Environment::Get("DATABASE_USER", util::Environment::Get("DB_USER", "postgres"));
    std::string password = util::Environment::Get("DATABASE_PASSWORD", util::Environment::Get("DB_PASSWORD", ""));

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

    const int max_retries = 2;
    const int retry_delay_sec = 2;

    for (int attempt = 1; attempt <= max_retries; ++attempt) {
        try {
            spdlog::info("Connecting to PostgreSQL (attempt {}/{})...", attempt, max_retries);
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
            last_error_ = "None (Connected)";

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
            return;
        } catch (const std::exception& e) {
            last_error_ = e.what();
            spdlog::warn("Database connection attempt {}/{} failed: {}", attempt, max_retries, e.what());
            if (attempt == max_retries) {
                // Safe user-readable error without exposing database passwords
                std::string safe_msg = "Database connection failed: unable to connect to host/port specified.";
                std::cerr << safe_msg << " Details: " << e.what() << std::endl;
                spdlog::error("{}. Error: {}", safe_msg, e.what());
                is_running_ = false;
                throw std::runtime_error(safe_msg);
            }
            std::this_thread::sleep_for(std::chrono::seconds(retry_delay_sec));
        }
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
        try {
            InitAndStart();
        } catch (...) {}
        lock.lock();
    }

    if (!is_running_) {
        throw std::runtime_error("Database connection is currently unavailable. Please verify database status.");
    }

    if (cv_.wait_for(lock, std::chrono::seconds(5), [this]() { return !pool_.empty() || !is_running_; })) {
        if (!is_running_) {
            throw std::runtime_error("Database connection pool is offline. Please check database status.");
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
    if (is_running_) {
        return std::make_shared<pqxx::connection>(connection_string_);
    }
    throw std::runtime_error("Database connection pool is offline. Please check database status.");
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
        if (!instance_ || !instance_->is_running_) {
            return false;
        }
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
