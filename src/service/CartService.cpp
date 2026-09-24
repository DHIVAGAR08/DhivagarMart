#include <cstdint>
#include "service/CartService.h"
#include "exception/AppException.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

CartService::CartService(std::shared_ptr<repository::ICartRepository> cart_repo,
                         std::shared_ptr<repository::IProductRepository> product_repo)
    : cart_repo_(std::move(cart_repo)), product_repo_(std::move(product_repo)) {}

dto::CartResponseDto CartService::GetCart(int64_t user_id, const std::string& request_id) {
    auto items = cart_repo_->GetCartItems(user_id, request_id);

    dto::CartResponseDto response;
    model::Money total{model::Money::FromCents(0)};
    int32_t total_count = 0;

    for (const auto& item : items) {
        response.items.push_back(dto::CartItemResponseDto::FromModel(item));
        total += item.GetSubtotal();
        total_count += item.quantity;
    }

    response.total_items = total_count;
    response.total_cents = total.GetCents();
    response.total_formatted = total.ToString();

    return response;
}

dto::CartResponseDto CartService::AddItem(int64_t user_id, const dto::AddCartItemRequestDto& req, const std::string& request_id) {
    spdlog::info("[{}] CartService::AddItem user={} product={} qty={}", request_id, user_id, req.product_id, req.quantity);

    if (req.product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }
    if (req.quantity <= 0) {
        throw exception::ValidationException("Quantity must be greater than zero");
    }

    auto product_opt = product_repo_->FindById(req.product_id, request_id);
    if (!product_opt.has_value()) {
        throw exception::NotFoundException("Product does not exist");
    }

    const auto& product = *product_opt;
    if (product.stock_qty < req.quantity) {
        throw exception::ValidationException("Requested quantity exceeds available stock (" + 
                                            std::to_string(product.stock_qty) + " available)");
    }

    cart_repo_->AddOrIncrementItem(user_id, req.product_id, req.quantity, request_id);
    return GetCart(user_id, request_id);
}

dto::CartResponseDto CartService::UpdateQuantity(int64_t user_id, int64_t product_id, const dto::UpdateCartItemRequestDto& req, const std::string& request_id) {
    spdlog::info("[{}] CartService::UpdateQuantity user={} product={} qty={}", request_id, user_id, product_id, req.quantity);

    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }

    if (req.quantity <= 0) {
        // Quantity 0 removes the item
        cart_repo_->RemoveItem(user_id, product_id, request_id);
        return GetCart(user_id, request_id);
    }

    auto product_opt = product_repo_->FindById(product_id, request_id);
    if (!product_opt.has_value()) {
        throw exception::NotFoundException("Product does not exist");
    }

    if (product_opt->stock_qty < req.quantity) {
        throw exception::ValidationException("Requested quantity exceeds available stock (" + 
                                            std::to_string(product_opt->stock_qty) + " available)");
    }

    if (!cart_repo_->UpdateQuantity(user_id, product_id, req.quantity, request_id)) {
        throw exception::NotFoundException("Item not found in cart");
    }

    return GetCart(user_id, request_id);
}

dto::CartResponseDto CartService::RemoveItem(int64_t user_id, int64_t product_id, const std::string& request_id) {
    spdlog::info("[{}] CartService::RemoveItem user={} product={}", request_id, user_id, product_id);
    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }

    cart_repo_->RemoveItem(user_id, product_id, request_id);
    return GetCart(user_id, request_id);
}

void CartService::ClearCart(int64_t user_id, const std::string& request_id) {
    cart_repo_->ClearCart(user_id, request_id);
}

} // namespace dhivagar::dhivagarmart::service
