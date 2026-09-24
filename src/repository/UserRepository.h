#pragma once

#include <cstdint>

#include "repository/IUserRepository.h"

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Concrete PostgreSQL implementation of IUserRepository using libpqxx parameterized queries.
 */
class UserRepository : public IUserRepository {
public:
    UserRepository() = default;
    ~UserRepository() override = default;

    model::User Create(const model::User& user, const std::string& request_id = "") override;
    std::optional<model::User> FindById(int64_t id, const std::string& request_id = "") override;
    std::optional<model::User> FindByEmail(const std::string& email, const std::string& request_id = "") override;
    std::vector<model::User> FindAll(const std::string& request_id = "") override;
};

} // namespace dhivagar::dhivagarmart::repository
