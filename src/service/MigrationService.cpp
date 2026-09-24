#include "service/MigrationService.h"
#include "plugin/DatabasePlugin.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <set>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace dhivagar::dhivagarmart::service {

struct MigrationItem {
    int version;
    std::string filename;
    fs::path full_path;

    bool operator<(const MigrationItem& other) const {
        return version < other.version;
    }
};

int MigrationService::RunPendingMigrations(const std::string& migrations_dir) {
    spdlog::info("Checking database migrations in directory: {}", migrations_dir);

    auto conn = plugin::DatabasePlugin::GetConnection();

    // 1. Ensure tracking table exists
    {
        pqxx::work tx(*conn);
        tx.exec(
            "CREATE TABLE IF NOT EXISTS schema_migrations ("
            "    version INT PRIMARY KEY,"
            "    name VARCHAR(255) NOT NULL,"
            "    applied_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP"
            ");"
        );
        tx.commit();
    }

    // 2. Fetch applied migration versions
    std::set<int> applied_versions;
    {
        pqxx::nontransaction ntx(*conn);
        auto res = ntx.exec("SELECT version FROM schema_migrations;");
        for (const auto& row : res) {
            applied_versions.insert(row["version"].as<int>());
        }
    }

    // 3. Scan directory for Vx__*.sql files
    if (!fs::exists(migrations_dir)) {
        spdlog::warn("Migrations directory {} does not exist, skipping.", migrations_dir);
        return 0;
    }

    std::vector<MigrationItem> available_migrations;
    static const std::regex kMigrationFileRegex(R"(^V(\d+)__(.+)\.sql$)");

    for (const auto& entry : fs::directory_iterator(migrations_dir)) {
        if (!entry.is_regular_file()) continue;
        std::string filename = entry.path().filename().string();
        std::smatch match;
        if (std::regex_match(filename, match, kMigrationFileRegex)) {
            int version = std::stoi(match[1].str());
            available_migrations.push_back({version, filename, entry.path()});
        }
    }

    std::sort(available_migrations.begin(), available_migrations.end());

    // 4. Apply unapplied migrations inside individual transactions
    int applied_count = 0;
    for (const auto& mig : available_migrations) {
        if (applied_versions.count(mig.version) > 0) {
            continue; // Already applied
        }

        spdlog::info("Applying database migration V{}: {}", mig.version, mig.filename);

        std::ifstream file(mig.full_path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not read migration file: " + mig.filename);
        }
        std::stringstream ss;
        ss << file.rdbuf();
        std::string sql = ss.str();

        try {
            pqxx::work tx(*conn);
            tx.exec(sql);
            tx.exec_params(
                "INSERT INTO schema_migrations (version, name, applied_at) VALUES ($1, $2, CURRENT_TIMESTAMP);",
                mig.version, mig.filename
            );
            tx.commit();
            applied_count++;
            spdlog::info("Successfully applied migration V{}: {}", mig.version, mig.filename);
        } catch (const std::exception& e) {
            spdlog::error("Migration failed on file {}: {}", mig.filename, e.what());
            throw std::runtime_error("Migration failed on " + mig.filename + ": " + e.what());
        }
    }

    spdlog::info("Migration runner completed. Applied {} new migration(s).", applied_count);
    return applied_count;
}

} // namespace dhivagar::dhivagarmart::service
