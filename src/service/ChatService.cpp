#include "service/ChatService.h"
#include "util/Environment.h"
#include "util/ValidationUtil.h"
#include "exception/AppException.h"
#include <spdlog/spdlog.h>
#include <algorithm>
#ifdef CPPHTTPLIB_USE_NON_BLOCKING_GETADDRINFO
#undef CPPHTTPLIB_USE_NON_BLOCKING_GETADDRINFO
#endif
#include <httplib.h>
#include <nlohmann/json.hpp>

namespace dhivagar::dhivagarmart::service {

// -------------------------------------------------------------
// MockChatProvider
// -------------------------------------------------------------
std::string MockChatProvider::GetReply(const std::string& user_message, const std::string& /*context*/) {
    std::string msg = user_message;
    std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);

    if (msg.find("hello") != std::string::npos || msg.find("hi") != std::string::npos || msg.find("hey") != std::string::npos) {
        return "Hello! Welcome to Dhivagar Mart. How can I help you find fresh groceries today?";
    }
    if (msg.find("fruit") != std::string::npos || msg.find("apple") != std::string::npos || msg.find("mango") != std::string::npos || msg.find("banana") != std::string::npos) {
        return "We offer premium fresh fruits! For example: Organic Gala Apples ($3.99/kg), Cavendish Bananas ($1.99/bunch), and Alphonso Mangoes ($8.99/box). Check out the Fruits category in our catalog!";
    }
    if (msg.find("milk") != std::string::npos || msg.find("dairy") != std::string::npos || msg.find("butter") != std::string::npos || msg.find("yogurt") != std::string::npos) {
        return "Our Dairy section includes Farm Fresh Whole Milk ($4.49/gallon), Organic Salted Butter ($5.29/500g), and Authentic Greek Yogurt ($5.99).";
    }
    if (msg.find("rice") != std::string::npos || msg.find("grain") != std::string::npos || msg.find("flour") != std::string::npos || msg.find("atta") != std::string::npos) {
        return "We stock Royal Aged Basmati Rice 5kg ($14.99), 100% Whole Wheat Atta 5kg ($9.49), and Organic Tri-Color Quinoa ($7.99).";
    }
    if (msg.find("delivery") != std::string::npos || msg.find("ship") != std::string::npos || msg.find("time") != std::string::npos) {
        return "Orders placed on Dhivagar Mart are packed fresh and typically delivered within 2 hours or same-day standard delivery!";
    }
    if (msg.find("order") != std::string::npos || msg.find("buy") != std::string::npos || msg.find("checkout") != std::string::npos) {
        return "To place an order: Browse products, add desired items to your cart, proceed to Checkout, enter your delivery address, and confirm your mock payment!";
    }
    if (msg.find("payment") != std::string::npos || msg.find("pay") != std::string::npos) {
        return "We currently support mock payment confirmation for instant, hassle-free checkout demonstrations.";
    }
    if (msg.find("seller") != std::string::npos || msg.find("sell") != std::string::npos) {
        return "Interested in selling on Dhivagar Mart? Register an account with the 'SELLER' role to list and manage your own grocery products and view incoming orders!";
    }
    if (msg.find("track") != std::string::npos || msg.find("status") != std::string::npos) {
        return "You can track your orders by navigating to 'My Orders' in the top menu to view real-time status: PENDING, CONFIRMED, SHIPPED, or DELIVERED.";
    }
    if (msg.find("review") != std::string::npos || msg.find("rating") != std::string::npos) {
        return "Verified buyers can submit a 1 to 5 star rating and comment on any product they have purchased from their order history.";
    }

    return "I am Dhivagar Mart's shopping assistant. You can ask me about our fresh fruits, dairy, rice, vegetables, snacks, prices, placing orders, delivery, or becoming a seller!";
}

// -------------------------------------------------------------
// GeminiChatProvider
// -------------------------------------------------------------
GeminiChatProvider::GeminiChatProvider(std::string api_key)
    : api_key_(std::move(api_key)), fallback_provider_(std::make_unique<MockChatProvider>()) {}

std::string GeminiChatProvider::GetReply(const std::string& user_message, const std::string& context) {
    if (api_key_.empty() || api_key_ == "CHANGE_ME") {
        return fallback_provider_->GetReply(user_message, context);
    }

    try {
        httplib::Client cli("https://generativelanguage.googleapis.com");
        cli.set_connection_timeout(std::chrono::seconds(5));
        cli.set_read_timeout(std::chrono::seconds(5));

        std::string prompt = 
            "System: You are Dhivagar Mart shopping assistant. "
            "Answer only questions related to Dhivagar Mart, its products, prices, categories, ordering, cart, checkout and order tracking. "
            "Do not answer unrelated questions.\n"
            "User: " + user_message;

        nlohmann::json payload = {
            {"contents", {
                {
                    {"parts", {
                        {{"text", prompt}}
                    }}
                }
            }}
        };

        std::string path = "/v1beta/models/gemini-1.5-flash:generateContent?key=" + api_key_;
        auto res = cli.Post(path, payload.dump(), "application/json");

        if (res && res->status == 200) {
            auto j = nlohmann::json::parse(res->body);
            if (j.contains("candidates") && !j["candidates"].empty() &&
                j["candidates"][0].contains("content") &&
                j["candidates"][0]["content"].contains("parts") &&
                !j["candidates"][0]["content"]["parts"].empty()) {
                return j["candidates"][0]["content"]["parts"][0]["text"].get<std::string>();
            }
        }
    } catch (const std::exception& e) {
        spdlog::warn("GeminiChatProvider outbound call failed: {}. Falling back to mock.", e.what());
    }

    return fallback_provider_->GetReply(user_message, context);
}

// -------------------------------------------------------------
// ChatProviderFactory
// -------------------------------------------------------------
std::unique_ptr<ChatProvider> ChatProviderFactory::CreateProvider() {
    std::string provider_type = util::Environment::Get("AI_CHATBOT_PROVIDER", "mock");
    std::transform(provider_type.begin(), provider_type.end(), provider_type.begin(), ::tolower);

    if (provider_type == "gemini") {
        std::string api_key = util::Environment::Get("AI_API_KEY", "");
        return std::make_unique<GeminiChatProvider>(api_key);
    }

    return std::make_unique<MockChatProvider>();
}

// -------------------------------------------------------------
// ChatService
// -------------------------------------------------------------
ChatService::ChatService(std::unique_ptr<ChatProvider> provider) {
    if (provider) {
        provider_ = std::move(provider);
    } else {
        provider_ = ChatProviderFactory::CreateProvider();
    }
}

bool ChatService::CheckRateLimit(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(rate_limit_mutex_);
    auto now = std::chrono::steady_clock::now();
    auto& timestamps = session_request_times_[session_id];

    // Remove entries older than 60 seconds
    while (!timestamps.empty() && now - timestamps.front() > std::chrono::seconds(60)) {
        timestamps.pop_front();
    }

    // Limit is 10 messages per minute
    if (timestamps.size() >= 10) {
        return false;
    }

    timestamps.push_back(now);
    return true;
}

dto::ChatResponseDto ChatService::Chat(const std::string& session_id, const std::string& user_message, const std::string& request_id) {
    spdlog::info("[{}] ChatService::Chat session={}", request_id, session_id);

    std::string clean_msg = util::ValidationUtil::Trim(user_message);
    if (clean_msg.empty()) {
        throw exception::ValidationException("Chat message cannot be empty");
    }
    if (clean_msg.length() > 500) {
        throw exception::ValidationException("Chat message exceeds maximum allowed length of 500 characters");
    }

    // Check sliding rate limit
    if (!CheckRateLimit(session_id)) {
        throw exception::ValidationException("Rate limit exceeded: You can only send up to 10 messages per minute.");
    }

    // Check repeated-question cache
    {
        std::lock_guard<std::mutex> lock(cache_mutex_);
        auto& user_cache = session_cache_[session_id];
        auto it = user_cache.find(clean_msg);
        if (it != user_cache.end()) {
            spdlog::debug("[{}] ChatService: Cache hit for question: '{}'", request_id, clean_msg);
            return dto::ChatResponseDto{
                .reply = it->second,
                .cached = true
            };
        }
    }

    // Generate reply via Provider
    std::string reply = provider_->GetReply(clean_msg, "");

    // Cache the reply
    {
        std::lock_guard<std::mutex> lock(cache_mutex_);
        session_cache_[session_id][clean_msg] = reply;
    }

    return dto::ChatResponseDto{
        .reply = reply,
        .cached = false
    };
}

} // namespace dhivagar::dhivagarmart::service
