#pragma once

#include <cstdint>

#include "repository/ICartRepository.h"
#include "repository/IProductRepository.h"
#include "dto/CartDtos.h"
#include <memory>
#include <string>

namespace dhivagar::dhivagarmart::service {

/**
 * @brief Service responsible for shopping cart management and running total calculations.
 */
class CartService {
public:
    CartService(std::shared_ptr<repository::ICartRepository> cart_repo,
                std::shared_ptr<repository::IProductRepository> product_repo);

    /**
     * @brief Retrieves active user shopping cart with running total calculation.
     * @param user_id Authenticated user ID.
     * @param request_id Correlation ID.
     * @return Cart response DTO with itemized details and total.
     */
    dto::CartResponseDto GetCart(int64_t user_id, const std::string& request_id = "");

    /**
     * @brief Adds or increments an item in the user's cart.
     * @param user_id User ID.
     * @param req Add item DTO.
     * @param request_id Correlation ID.
     * @throws ValidationException on invalid quantity or insufficient stock.
     * @throws NotFoundException if product does not exist.
     */
    dto::CartResponseDto AddItem(int64_t user_id, const dto::AddCartItemRequestDto& req, const std::string& request_id = "");

    /**
     * @brief Updates quantity for a specific product in the cart.
     * @param user_id User ID.
     * @param product_id Product ID.
     * @param req Update quantity DTO.
     * @param request_id Correlation ID.
     * @throws ValidationException on invalid quantity or stock shortage.
     */
    dto::CartResponseDto UpdateQuantity(int64_t user_id, int64_t product_id, const dto::UpdateCartItemRequestDto& req, const std::string& request_id = "");

    /**
     * @brief Removes a product from the user's cart.
     * @param user_id User ID.
     * @param product_id Product ID.
     * @param request_id Correlation ID.
     */
    dto::CartResponseDto RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id = "");

    /**
     * @brief Clears all items from the cart.
     */
    void ClearCart(int64_t user_id, const std::string& request_id = "");

private:
    std::shared_ptr<repository::ICartRepository> cart_repo_;
    std::shared_ptr<repository::IProductRepository> product_repo_;
};

} // namespace dhivagar::dhivagarmart::service
