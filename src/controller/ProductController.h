#pragma once

#include <cstdint>

#include <drogon/HttpController.h>
#include "service/ProductService.h"
#include <memory>

namespace dhivagar::dhivagarmart::controller {

/**
 * @brief Controller handling public product catalog retrieval, filtering, and detail views.
 */
class ProductController : public drogon::HttpController<ProductController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ProductController::GetProducts, "/api/v1/products", drogon::Get, "dhivagar::dhivagarmart::filter::LoggingFilter");
    ADD_METHOD_TO(ProductController::GetProductById, "/api/v1/products/{1}", drogon::Get, "dhivagar::dhivagarmart::filter::LoggingFilter");
    METHOD_LIST_END

    ProductController();

    void GetProducts(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void GetProductById(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int64_t id);

private:
    std::shared_ptr<service::ProductService> product_service_;
};

} // namespace dhivagar::dhivagarmart::controller
