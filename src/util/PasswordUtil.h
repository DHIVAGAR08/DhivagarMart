#pragma once

#include <string>

namespace dhivagar::dhivagarmart::util {

class PasswordUtil {
public:
    static bool Initialize();
    static std::string HashPassword(const std::string& plaintext_password);
    static bool VerifyPassword(const std::string& hash, const std::string& plaintext_password);
};

} // namespace dhivagar::dhivagarmart::util
