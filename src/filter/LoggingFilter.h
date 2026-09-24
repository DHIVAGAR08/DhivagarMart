#pragma once

#include <drogon/HttpFilter.h>

namespace dhivagar::dhivagarmart::filter {

/**
 * @brief Filter that generates correlation request IDs and performs structured logging.
 */
class LoggingFilter : public drogon::HttpFilter<LoggingFilter> {
public:
    LoggingFilter() = default;
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};

} // namespace dhivagar::dhivagarmart::filter
