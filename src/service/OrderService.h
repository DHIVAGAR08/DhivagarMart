#pragma once

#include <cstdint>

#include "repository/IOrderRepository.h"
#include "repository/ICartRepository.h"
#include "repository/IProductRepository.h"
#include "dto/OrderDtos.h"
#include <memory>
#include <vector>
#include <string>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Service orchestrating order placement and checkout transactions with ACID guarantees.
 */
class OrderService {
public:
    OrderService(std::shared_ptr<repository::IOrderRepository> order_repo,
                 std::shared_ptr<repository::ICartRepository> cart_repo,
                 std::shared_ptr<repository::IProductRepository> product_repo);

    /**
     * @brief Executes atomic checkout: cart validation -> stock check -> mock payment -> order & items creation -> stock reduction -> cart purge -> commit.
     * @param buyer_id Authenticated buyer ID.
     * @param req Checkout request details.
     * @param request_id Correlation ID.
     * @return Created Order response DTO.
     * @throws ValidationException on empty cart or stock shortages.
     */
    dto::OrderResponseDto Checkout(int64_t buyer_id, const dto::CreateOrderRequestDto& req, const std::string& request_id = "");

    /**
     * @brief Retrieves all orders placed by the specified buyer.
     */
    std::vector<dto::OrderResponseDto> GetBuyerOrders(int64_t buyer_id, const std::string& request_id = "");

    /**
     * @brief Retrieves a specific order by ID, verifying buyer ownership or admin rights.
     */
    dto::OrderResponseDto GetOrderById(int64_t order_id, int64_t user_id, bool is_admin, const std::string& request_id = "");

    /**
     * @brief Cancels an order, verifies permissions, restores inventory, and updates status to CANCELLED.
     */
    dto::OrderResponseDto CancelOrder(int64_t order_id, int64_t user_id, bool is_admin, const std::string& request_id = "");

private:
    std::shared_ptr<repository::IOrderRepository> order_repo_;
    std::shared_ptr<repository::ICartRepository> cart_repo_;
    std::shared_ptr<repository::IProductRepository> product_repo_;
};

} // namespace dhivagar::dhivagarmart::service
