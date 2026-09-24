#include <cstdint>
#include "filter/LoggingFilter.h"
#include <drogon/utils/Utilities.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

namespace dhivagar::dhivagarmart::filter {

static std::string GenerateRequestId() {
    static thread_local std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<uint64_t> dist;
    std::ostringstream oss;
    oss << "req-" << std::hex << std::setw(16) << std::setfill('0') << dist(rng);
    return oss.str();
}

void LoggingFilter::doFilter(const drogon::HttpRequestPtr &req,
                             drogon::FilterCallback &&fcb,
                             drogon::FilterChainCallback &&fccb) {
    std::string request_id = GenerateRequestId();
    req->getAttributes()->insert("request_id", request_id);

    spdlog::info("[{}] --> {} {} from {}", 
                 request_id, req->getMethodString(), req->getPath(), req->getPeerAddr().toIpPort());

    fccb();
}

} // namespace dhivagar::dhivagarmart::filter
