#include "util/ValidationUtil.h"
#include <regex>
#include <algorithm>
#include <cstdint>

namespace dhivagar::dhivagarmart::util {

bool ValidationUtil::IsValidEmail(const std::string& email) {
    if (email.empty() || email.length() > 254) {
        return false;
    }
    static const std::regex kEmailRegex(
        R"(^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)+$)"
    );
    return std::regex_match(email, kEmailRegex);
}

bool ValidationUtil::IsValidPassword(const std::string& password) {
    return password.length() >= 6;
}

bool ValidationUtil::IsValidRole(const std::string& role) {
    return role == "BUYER" || role == "SELLER";
}

bool ValidationUtil::IsValidRating(int32_t rating) {
    return rating >= 1 && rating <= 5;
}

std::string ValidationUtil::EscapeHtml(const std::string& input) {
    std::string escaped;
    escaped.reserve(input.size() * 1.1);
    for (char c : input) {
        switch (c) {
            case '&':  escaped.append("&amp;"); break;
            case '<':  escaped.append("&lt;"); break;
            case '>':  escaped.append("&gt;"); break;
            case '"':  escaped.append("&quot;"); break;
            case '\'': escaped.append("&#x27;"); break;
            case '/':  escaped.append("&#x2F;"); break;
            default:   escaped.push_back(c); break;
        }
    }
    return escaped;
}

std::string ValidationUtil::Trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
        return std::isspace(c);
    }).base();
    return (start < end) ? std::string(start, end) : std::string();
}

} // namespace dhivagar::dhivagarmart::util
