#pragma once

#include <cstdint>

#include "repository/IProductRepository.h"
#include "repository/IOrderRepository.h"
#include "dto/ProductDtos.h"
#include "dto/OrderDtos.h"
#include <memory>
#include <vector>
#include <string>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Service providing seller catalog management and incoming order fulfillment operations.
 */
class SellerService {
public:
    SellerService(std::shared_ptr<repository::IProductRepository> product_repo,
                  std::shared_ptr<repository::IOrderRepository> order_repo);

    /**
     * @brief Retrieves products listed by the authenticated seller.
     */
    std::vector<dto::ProductResponseDto> GetSellerProducts(int64_t seller_id, const std::string& request_id = "");

    /**
     * @brief Creates a new product listing owned by the seller.
     */
    dto::ProductResponseDto CreateProduct(int64_t seller_id, const dto::ProductRequestDto& req, const std::string& request_id = "");

    /**
     * @brief Updates an existing product listing. Enforces seller ownership.
     */
    dto::ProductResponseDto UpdateProduct(int64_t seller_id, int64_t product_id, const dto::ProductRequestDto& req, const std::string& request_id = "");

    /**
     * @brief Deletes a product listing. Enforces seller ownership.
     */
    void DeleteProduct(int64_t seller_id, int64_t product_id, const std::string& request_id = "");

    /**
     * @brief Retrieves incoming orders containing products owned by this seller.
     */
    std::vector<dto::OrderResponseDto> GetSellerOrders(int64_t seller_id, const std::string& request_id = "");

    /**
     * @brief Updates status of an incoming seller order.
     */
    dto::OrderResponseDto UpdateOrderStatus(int64_t seller_id, int64_t order_id, const std::string& new_status, const std::string& request_id = "");

private:
    std::shared_ptr<repository::IProductRepository> product_repo_;
    std::shared_ptr<repository::IOrderRepository> order_repo_;
};

} // namespace dhivagar::dhivagarmart::service
