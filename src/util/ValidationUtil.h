#pragma once

#include <string>
#include <cstdint>

namespace dhivagar::dhivagarmart::util {

class ValidationUtil {
public:
    static bool IsValidEmail(const std::string& email);
    static bool IsValidPassword(const std::string& password);
    static bool IsValidRole(const std::string& role);
    static bool IsValidRating(int32_t rating);
    static std::string EscapeHtml(const std::string& input);
    static std::string Trim(const std::string& str);
};

} // namespace dhivagar::dhivagarmart::util
