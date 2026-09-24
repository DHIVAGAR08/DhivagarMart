#include "filter/AuthFilter.h"
#include "util/JsonUtil.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::filter {

void AuthFilter::doFilter(const drogon::HttpRequestPtr &req,
                          drogon::FilterCallback &&fcb,
                          drogon::FilterChainCallback &&fccb) {
    auto session = req->session();
    if (!session || !session->find("user_id")) {
        auto resp = util::JsonUtil::CreateErrorResponse(
            401, "UNAUTHENTICATED", "Authentication required. Please log in to continue."
        );
        fcb(resp);
        return;
    }
    fccb();
}

void SellerFilter::doFilter(const drogon::HttpRequestPtr &req,
                            drogon::FilterCallback &&fcb,
                            drogon::FilterChainCallback &&fccb) {
    auto session = req->session();
    if (!session || !session->find("user_id")) {
        auto resp = util::JsonUtil::CreateErrorResponse(
            401, "UNAUTHENTICATED", "Authentication required. Please log in as a seller."
        );
        fcb(resp);
        return;
    }

    std::string role = session->get<std::string>("role");
    if (role != "SELLER" && role != "ADMIN") {
        auto resp = util::JsonUtil::CreateErrorResponse(
            403, "FORBIDDEN", "Access denied. Seller or Admin privileges required."
        );
        fcb(resp);
        return;
    }

    fccb();
}

void AdminFilter::doFilter(const drogon::HttpRequestPtr &req,
                           drogon::FilterCallback &&fcb,
                           drogon::FilterChainCallback &&fccb) {
    auto session = req->session();
    if (!session || !session->find("user_id")) {
        auto resp = util::JsonUtil::CreateErrorResponse(
            401, "UNAUTHENTICATED", "Authentication required. Please log in as an administrator."
        );
        fcb(resp);
        return;
    }

    std::string role = session->get<std::string>("role");
    if (role != "ADMIN") {
        auto resp = util::JsonUtil::CreateErrorResponse(
            403, "FORBIDDEN", "Access denied. Administrator privileges required."
        );
        fcb(resp);
        return;
    }

    fccb();
}

} // namespace dhivagar::dhivagarmart::filter
