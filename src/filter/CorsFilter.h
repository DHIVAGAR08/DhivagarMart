#pragma once

#include <drogon/HttpFilter.h>

namespace dhivagar::dhivagarmart::filter {

/**
 * @brief Filter handling CORS headers and OPTIONS pre-flight requests.
 */
class CorsFilter : public drogon::HttpFilter<CorsFilter> {
public:
    CorsFilter() = default;
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};

} // namespace dhivagar::dhivagarmart::filter
