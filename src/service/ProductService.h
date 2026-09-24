#pragma once

#include <cstdint>

#include "repository/IProductRepository.h"
#include "dto/ProductDtos.h"
#include <memory>
#include <vector>
#include <optional>
#include <string>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Service providing catalog browsing and public product search operations.
 */
class ProductService {
public:
    explicit ProductService(std::shared_ptr<repository::IProductRepository> product_repo);

    /**
     * @brief Retrieves filtered or searched product catalog listings.
     * @param category Optional category filter.
     * @param search Optional keyword search term.
     * @param request_id Correlation ID.
     * @return List of product response DTOs.
     */
    std::vector<dto::ProductResponseDto> GetProducts(
        const std::optional<std::string>& category,
        const std::optional<std::string>& search,
        const std::string& request_id = "");

    /**
     * @brief Retrieves product details by ID.
     * @param id Product ID.
     * @param request_id Correlation ID.
     * @return Product response DTO.
     * @throws NotFoundException if product does not exist.
     */
    dto::ProductResponseDto GetProductById(int64_t id, const std::string& request_id = "");

private:
    std::shared_ptr<repository::IProductRepository> product_repo_;
};

} // namespace dhivagar::dhivagarmart::service
