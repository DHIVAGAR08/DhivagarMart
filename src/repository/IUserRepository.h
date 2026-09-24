#pragma once

#include <cstdint>

#include "model/User.h"
#include <optional>
#include <vector>
#include <string>
#include <memory>

namespace dhivagar::dhivagarmart::repository {

/**
 * @brief Interface for User data access operations.
 */
class IUserRepository {
public:
    virtual ~IUserRepository() = default;

    /**
     * @brief Creates a new user record.
     * @param user User entity to persist.
     * @return Persisted User entity with assigned ID.
     */
    virtual model::User Create(const model::User& user, const std::string& request_id = "") = 0;

    /**
     * @brief Finds a user by unique identifier.
     * @param id User ID.
     * @return User if found, std::nullopt otherwise.
     */
    virtual std::optional<model::User> FindById(int64_t id, const std::string& request_id = "") = 0;

    /**
     * @brief Finds a user by email address.
     * @param email User email.
     * @return User if found, std::nullopt otherwise.
     */
    virtual std::optional<model::User> FindByEmail(const std::string& email, const std::string& request_id = "") = 0;

    /**
     * @brief Retrieves all registered users (for admin review).
     * @return List of all users.
     */
    virtual std::vector<model::User> FindAll(const std::string& request_id = "") = 0;
};

} // namespace dhivagar::dhivagarmart::repository
