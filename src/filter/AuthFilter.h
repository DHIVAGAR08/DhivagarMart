#pragma once

#include <drogon/HttpFilter.h>

namespace dhivagar::dhivagarmart::filter {

/**
 * @brief Filter ensuring user is authenticated via an active Drogon session.
 */
class AuthFilter : public drogon::HttpFilter<AuthFilter> {
public:
    AuthFilter() = default;
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};

/**
 * @brief Filter ensuring user has SELLER or ADMIN role.
 */
class SellerFilter : public drogon::HttpFilter<SellerFilter> {
public:
    SellerFilter() = default;
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};

/**
 * @brief Filter ensuring user has ADMIN role.
 */
class AdminFilter : public drogon::HttpFilter<AdminFilter> {
public:
    AdminFilter() = default;
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fccb) override;
};

} // namespace dhivagar::dhivagarmart::filter
