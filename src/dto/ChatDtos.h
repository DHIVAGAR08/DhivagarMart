#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace dhivagar::dhivagarmart::dto {

struct ChatRequestDto {
    std::string message;
};

inline void from_json(const nlohmann::json& j, ChatRequestDto& dto) {
    dto.message = j.value("message", "");
}

struct ChatResponseDto {
    std::string reply;
    bool cached{false};
};

inline void to_json(nlohmann::json& j, const ChatResponseDto& dto) {
    j = nlohmann::json{
        {"reply", dto.reply},
        {"cached", dto.cached}
    };
}

} // namespace dhivagar::dhivagarmart::dto
