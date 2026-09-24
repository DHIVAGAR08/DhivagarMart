#pragma once

#include <string>
#include <vector>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Service responsible for discovering and executing pending database migrations.
 */
class MigrationService {
public:
    /**
     * @brief Scans the specified migrations directory and applies any unapplied SQL migrations in transactions.
     * @param migrations_dir Path to migrations folder (e.g. "db/migrations").
     * @return Number of applied migrations.
     */
    static int RunPendingMigrations(const std::string& migrations_dir = "db/migrations");
};

} // namespace dhivagar::dhivagarmart::service
