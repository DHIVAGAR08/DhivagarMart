#pragma once

#include <drogon/plugins/Plugin.h>
#include <pqxx/pqxx>
#include <memory>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace dhivagar::dhivagarmart::plugin {

/**
 * @brief RAII connection handle that returns the libpqxx connection to the pool on destruction.
 */
class ConnectionHandle {
public:
    ConnectionHandle(std::shared_ptr<pqxx::connection> conn, 
                     std::function<void(std::shared_ptr<pqxx::connection>)> return_fn)
        : conn_(std::move(conn)), return_fn_(std::move(return_fn)) {}

    ~ConnectionHandle() {
        if (conn_ && return_fn_) {
            return_fn_(std::move(conn_));
        }
    }

    // Move-only semantics
    ConnectionHandle(const ConnectionHandle&) = delete;
    ConnectionHandle& operator=(const ConnectionHandle&) = delete;
    ConnectionHandle(ConnectionHandle&&) noexcept = default;
    ConnectionHandle& operator=(ConnectionHandle&&) noexcept = default;

    pqxx::connection& operator*() { return *conn_; }
    pqxx::connection* operator->() { return conn_.get(); }
    [[nodiscard]] pqxx::connection& Get() { return *conn_; }

private:
    std::shared_ptr<pqxx::connection> conn_;
    std::function<void(std::shared_ptr<pqxx::connection>)> return_fn_;
};

/**
 * @brief Drogon Plugin managing the PostgreSQL connection pool lifecycle.
 * Initializes pool at startup, verifies with SELECT 1, and provides thread-safe RAII connection handles.
 */
class DatabasePlugin : public drogon::Plugin<DatabasePlugin> {
public:
    DatabasePlugin();
    ~DatabasePlugin() override;

    void initAndStart(const Json::Value &config) override;
    void shutdown() override;

    // Direct lifecycle triggers for testing or standalone modes
    static void InitAndStart();
    static void Shutdown();

    /**
     * @brief Acquires a connection from the pool wrapped in an RAII handle.
     */
    static ConnectionHandle GetConnection();

    /**
     * @brief Executes a trivial query (SELECT 1) to verify database health.
     */
    static bool CheckHealth();

    [[nodiscard]] static std::string BuildConnectionString();

private:
    std::shared_ptr<pqxx::connection> AcquireInternal();
    void ReleaseInternal(std::shared_ptr<pqxx::connection> conn);

    static DatabasePlugin* instance_;
    std::string connection_string_;
    size_t min_connections_{4};
    size_t max_connections_{20};
    std::queue<std::shared_ptr<pqxx::connection>> pool_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool is_running_{false};
};

} // namespace dhivagar::dhivagarmart::plugin
