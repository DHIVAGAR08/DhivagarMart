#include <cstdint>
#include "service/ProductService.h"
#include "exception/AppException.h"
#include "util/ValidationUtil.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

ProductService::ProductService(std::shared_ptr<repository::IProductRepository> product_repo)
    : product_repo_(std::move(product_repo)) {}

std::vector<dto::ProductResponseDto> ProductService::GetProducts(
    const std::optional<std::string>& category,
    const std::optional<std::string>& search,
    const std::string& request_id) {

    std::optional<std::string> clean_category;
    if (category.has_value() && !category->empty() && *category != "All") {
        clean_category = util::ValidationUtil::Trim(*category);
    }

    std::optional<std::string> clean_search;
    if (search.has_value() && !search->empty()) {
        clean_search = util::ValidationUtil::Trim(*search);
    }

    auto products = product_repo_->FindAll(clean_category, clean_search, request_id);

    std::vector<dto::ProductResponseDto> dtos;
    dtos.reserve(products.size());
    for (const auto& prod : products) {
        dtos.push_back(dto::ProductResponseDto::FromModel(prod));
    }
    return dtos;
}

dto::ProductResponseDto ProductService::GetProductById(int64_t id, const std::string& request_id) {
    if (id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }

    auto product_opt = product_repo_->FindById(id, request_id);
    if (!product_opt.has_value()) {
        throw exception::NotFoundException("Product not found");
    }

    return dto::ProductResponseDto::FromModel(*product_opt);
}

} // namespace dhivagar::dhivagarmart::service
