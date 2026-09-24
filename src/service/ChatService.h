#pragma once

#include "dto/ChatDtos.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <deque>
#include <chrono>
#include <mutex>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Strategy pattern interface for AI chat generation providers.
 */
class ChatProvider {
public:
    virtual ~ChatProvider() = default;
    virtual std::string GetReply(const std::string& user_message, const std::string& context) = 0;
};

/**
 * @brief Mock Chat Provider providing canned FAQ answers without outbound network dependencies.
 */
class MockChatProvider : public ChatProvider {
public:
    MockChatProvider() = default;
    ~MockChatProvider() override = default;
    std::string GetReply(const std::string& user_message, const std::string& context) override;
};

/**
 * @brief Real Gemini Chat Provider communicating with Google Gemini API via cpp-httplib.
 */
class GeminiChatProvider : public ChatProvider {
public:
    explicit GeminiChatProvider(std::string api_key);
    ~GeminiChatProvider() override = default;
    std::string GetReply(const std::string& user_message, const std::string& context) override;

private:
    std::string api_key_;
    std::unique_ptr<MockChatProvider> fallback_provider_;
};

/**
 * @brief Factory for instantiating the configured ChatProvider.
 */
class ChatProviderFactory {
public:
    static std::unique_ptr<ChatProvider> CreateProvider();
};

/**
 * @brief Service managing chatbot rate limits, caching, and conversational dispatch.
 */
class ChatService {
public:
    explicit ChatService(std::unique_ptr<ChatProvider> provider = nullptr);

    /**
     * @brief Processes a user message from a session with rate limiting, length limits, and cache.
     * @param session_id Drogon cookie session ID.
     * @param user_message Raw customer message string.
     * @param request_id Correlation ID.
     * @return ChatResponseDto with response and cached indicator.
     */
    dto::ChatResponseDto Chat(const std::string& session_id, const std::string& user_message, const std::string& request_id = "");

private:
    std::unique_ptr<ChatProvider> provider_;

    // Per-session rate limiter: max 10 messages per minute
    std::mutex rate_limit_mutex_;
    std::unordered_map<std::string, std::deque<std::chrono::steady_clock::time_point>> session_request_times_;

    // Per-session question cache: session_id -> (question -> answer)
    std::mutex cache_mutex_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> session_cache_;

    bool CheckRateLimit(const std::string& session_id);
};

} // namespace dhivagar::dhivagarmart::service
