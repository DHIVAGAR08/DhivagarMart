#include "filter/CorsFilter.h"
#include <drogon/HttpResponse.h>

namespace dhivagar::dhivagarmart::filter {

void CorsFilter::doFilter(const drogon::HttpRequestPtr &req,
                          drogon::FilterCallback &&fcb,
                          drogon::FilterChainCallback &&fccb) {
    if (req->method() == drogon::Options) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k200OK);
        resp->addHeader("Access-Control-Allow-Origin", req->getHeader("Origin").empty() ? "*" : req->getHeader("Origin"));
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");
        resp->addHeader("Access-Control-Allow-Credentials", "true");
        resp->addHeader("Access-Control-Max-Age", "86400");
        fcb(resp);
        return;
    }
    fccb();
}

} // namespace dhivagar::dhivagarmart::filter
