#include <cstdint>
#include "repository/UserRepository.h"
#include "plugin/DatabasePlugin.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::repository {

static model::User RowToUser(const pqxx::row_ref& row) {
    model::User u;
    u.id = row["id"].as<int64_t>();
    u.name = row["name"].as<std::string>();
    u.email = row["email"].as<std::string>();
    u.password_hash = row["password_hash"].as<std::string>();
    u.role = model::StringToUserRole(row["role"].as<std::string>());
    u.created_at = row["created_at"].as<std::string>();
    return u;
}

model::User UserRepository::Create(const model::User& user, const std::string& request_id) {
    spdlog::debug("[{}] UserRepository::Create user: {}", request_id, user.email);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::work tx(*conn);

    std::string role_str = model::UserRoleToString(user.role);
    auto res = tx.exec_params(
        "INSERT INTO users (name, email, password_hash, role) "
        "VALUES ($1, $2, $3, $4) "
        "RETURNING id, name, email, password_hash, role, created_at;",
        user.name, user.email, user.password_hash, role_str
    );
    tx.commit();

    if (res.empty()) {
        throw std::runtime_error("Failed to insert user record");
    }
    return RowToUser(res[0]);
}

std::optional<model::User> UserRepository::FindById(int64_t id, const std::string& request_id) {
    spdlog::debug("[{}] UserRepository::FindById: {}", request_id, id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT id, name, email, password_hash, role, created_at "
        "FROM users WHERE id = $1;",
        id
    );

    if (res.empty()) {
        return std::nullopt;
    }
    return RowToUser(res[0]);
}

std::optional<model::User> UserRepository::FindByEmail(const std::string& email, const std::string& request_id) {
    spdlog::debug("[{}] UserRepository::FindByEmail: {}", request_id, email);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT id, name, email, password_hash, role, created_at "
        "FROM users WHERE LOWER(email) = LOWER($1);",
        email
    );

    if (res.empty()) {
        return std::nullopt;
    }
    return RowToUser(res[0]);
}

std::vector<model::User> UserRepository::FindAll(const std::string& request_id) {
    spdlog::debug("[{}] UserRepository::FindAll", request_id);
    auto conn = plugin::DatabasePlugin::GetConnection();
    pqxx::nontransaction ntx(*conn);

    auto res = ntx.exec_params(
        "SELECT id, name, email, password_hash, role, created_at "
        "FROM users ORDER BY id ASC;"
    );

    std::vector<model::User> users;
    users.reserve(res.size());
    for (const auto& row : res) {
        users.push_back(RowToUser(row));
    }
    return users;
}

} // namespace dhivagar::dhivagarmart::repository
