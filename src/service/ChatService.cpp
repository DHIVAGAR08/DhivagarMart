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
        return "Hello! Welcome to Dhivagar Mart. How can I help you find smartphones, laptops, electronics, and tech accessories today?";
    }
    if (msg.find("mobile") != std::string::npos || msg.find("phone") != std::string::npos || msg.find("samsung") != std::string::npos || msg.find("iphone") != std::string::npos || msg.find("oneplus") != std::string::npos || msg.find("redmi") != std::string::npos) {
        return "We offer flagship smartphones including Samsung Galaxy S24 Ultra 5G, Apple iPhone 15 Pro Max, OnePlus 12 5G, and Redmi Note 13 Pro+. Explore the Mobiles category in our catalog!";
    }
    if (msg.find("laptop") != std::string::npos || msg.find("dell") != std::string::npos || msg.find("hp") != std::string::npos || msg.find("lenovo") != std::string::npos || msg.find("asus") != std::string::npos || msg.find("acer") != std::string::npos) {
        return "Our Laptops category features top models: Dell Inspiron 15, HP Pavilion 15, Lenovo IdeaPad Slim 3, ASUS Vivobook 16X, and Acer Aspire 5.";
    }
    if (msg.find("electrical") != std::string::npos || msg.find("tv") != std::string::npos || msg.find("kettle") != std::string::npos || msg.find("fan") != std::string::npos || msg.find("ac") != std::string::npos || msg.find("air conditioner") != std::string::npos) {
        return "In Electrical appliances, we offer 55-inch 4K Smart TVs, 1.5 Ton Inverter ACs, Fast-Boil Kettles, Silent BLDC Ceiling Fans, and 750W Mixer Grinders.";
    }
    if (msg.find("audio") != std::string::npos || msg.find("headphone") != std::string::npos || msg.find("earbud") != std::string::npos || msg.find("speaker") != std::string::npos || msg.find("soundbar") != std::string::npos) {
        return "Explore high-fidelity Audio gear including ANC Wireless Earbuds, Studio Headphones, 20W Waterproof Bluetooth Speakers, and 160W Dolby Soundbars.";
    }
    if (msg.find("watch") != std::string::npos || msg.find("smart watch") != std::string::npos || msg.find("fitness") != std::string::npos) {
        return "Track your fitness with Smart Watches including Waterproof GPS Fitness Watches, Bluetooth Calling Smartwatches, and Luxury AMOLED Smart Watches.";
    }
    if (msg.find("gaming") != std::string::npos || msg.find("keyboard") != std::string::npos || msg.find("mouse") != std::string::npos || msg.find("controller") != std::string::npos) {
        return "Level up with Gaming gear: 16000 DPI Optical Gaming Mouse, TKL Mechanical RGB Keyboard, Vibration Game Controller, and 7.1 Surround Gaming Headsets.";
    }
    if (msg.find("camera") != std::string::npos || msg.find("webcam") != std::string::npos) {
        return "Capture every moment with 4K Mirrorless Cameras, 4K 60FPS Action Cams, and 1080P HD Streaming Webcams.";
    }
    if (msg.find("accessory") != std::string::npos || msg.find("accessories") != std::string::npos || msg.find("cable") != std::string::npos || msg.find("power bank") != std::string::npos || msg.find("backpack") != std::string::npos) {
        return "Check out Tech Accessories: 100W Fast Charging Braided USB-C Cables, 20000mAh Power Banks, Ergonomic Wireless Mice, and Padded Laptop Backpacks.";
    }
    if (msg.find("fruit") != std::string::npos || msg.find("grocery") != std::string::npos || msg.find("dairy") != std::string::npos || msg.find("rice") != std::string::npos) {
        return "Dhivagar Mart has upgraded to a modern Electronics & Technology marketplace! We now offer Mobiles, Laptops, Electrical appliances, Audio, Smart Watches, Gaming, Cameras, and Accessories.";
    }
    if (msg.find("delivery") != std::string::npos || msg.find("ship") != std::string::npos || msg.find("time") != std::string::npos) {
        return "Orders placed on Dhivagar Mart are processed promptly and shipped with expedited express tech delivery!";
    }
    if (msg.find("order") != std::string::npos || msg.find("buy") != std::string::npos || msg.find("checkout") != std::string::npos) {
        return "To place an order: Browse products, add desired items to your cart, proceed to Checkout, enter your delivery address, and confirm your mock payment!";
    }
    if (msg.find("payment") != std::string::npos || msg.find("pay") != std::string::npos) {
        return "We support Cash on Delivery (COD), UPI (GPay/PhonePe/Paytm), and Credit/Debit Cards for fast, secure checkout in Indian Rupees (₹).";
    }
    if (msg.find("seller") != std::string::npos || msg.find("sell") != std::string::npos) {
        return "Interested in selling tech on Dhivagar Mart? Register an account with the 'SELLER' role to list and manage your electronics catalog and view incoming orders!";
    }
    if (msg.find("track") != std::string::npos || msg.find("status") != std::string::npos) {
        return "You can track your orders by navigating to 'My Orders' in the top menu to view real-time status: PENDING, CONFIRMED, SHIPPED, or DELIVERED.";
    }
    if (msg.find("review") != std::string::npos || msg.find("rating") != std::string::npos) {
        return "Verified buyers can submit a 1 to 5 star rating and comment on any product they have purchased from their order history.";
    }

    return "I am Dhivagar Mart's technology shopping assistant. You can ask me about mobiles, laptops, electrical appliances, audio, smart watches, gaming gear, cameras, accessories, prices, placing orders, or becoming a seller!";
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
