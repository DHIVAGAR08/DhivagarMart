#include <cstdint>
#include "service/OrderService.h"
#include "plugin/DatabasePlugin.h"
#include "exception/AppException.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

OrderService::OrderService(std::shared_ptr<repository::IOrderRepository> order_repo,
                           std::shared_ptr<repository::ICartRepository> cart_repo,
                           std::shared_ptr<repository::IProductRepository> product_repo)
    : order_repo_(std::move(order_repo)),
      cart_repo_(std::move(cart_repo)),
      product_repo_(std::move(product_repo)) {}

dto::OrderResponseDto OrderService::Checkout(int64_t buyer_id, const dto::CreateOrderRequestDto& req, const std::string& request_id) {
    spdlog::info("[{}] OrderService::Checkout started for buyer_id={}", request_id, buyer_id);

    // 1. Retrieve cart items
    auto cart_items = cart_repo_->GetCartItems(buyer_id, request_id);
    if (cart_items.empty()) {
        throw exception::ValidationException("Cannot checkout with an empty cart");
    }

    // 2. Validate stock and calculate total using Money value object
    model::Money total_amount{model::Money::FromCents(0)};
    for (const auto& item : cart_items) {
        if (item.quantity <= 0) {
            throw exception::ValidationException("Invalid item quantity in cart");
        }
        if (item.available_stock < item.quantity) {
            throw exception::ValidationException(
                "Insufficient stock for '" + item.product_name + "'. Available: " + 
                std::to_string(item.available_stock) + ", requested: " + std::to_string(item.quantity)
            );
        }
        total_amount += item.GetSubtotal();
    }

    // 3. Mock Payment Confirmation step
    spdlog::info("[{}] Mock payment processed successfully for amount {}", request_id, total_amount.ToString());

    // 4. Atomic Database Transaction for Order creation, Stock reduction, Cart clear
    int64_t order_id = 0;
    try {
        auto conn = plugin::DatabasePlugin::GetConnection();
        pqxx::work tx(*conn);

        // a. Create Order header
        order_id = order_repo_->CreateOrderInTransaction(
            tx, buyer_id, total_amount.GetCents(), model::OrderStatus::kConfirmed, request_id
        );

        // b. Create Order items and reduce stock
        for (const auto& item : cart_items) {
            order_repo_->CreateOrderItemInTransaction(
                tx, order_id, item.product_id, item.quantity, item.product_price.GetCents(), request_id
            );

            bool stock_updated = product_repo_->ReduceStockInTransaction(
                tx, item.product_id, item.quantity, request_id
            );
            if (!stock_updated) {
                throw exception::ValidationException(
                    "Concurrent inventory conflict: insufficient stock remaining for '" + item.product_name + "'"
                );
            }
        }

        // c. Clear buyer's cart
        cart_repo_->ClearCartInTransaction(tx, buyer_id, request_id);

        // d. Commit transaction
        tx.commit();
        spdlog::info("[{}] Order checkout committed successfully: order_id={}", request_id, order_id);
    } catch (const exception::AppException&) {
        // Transaction auto-rolls back on exception scope exit
        spdlog::warn("[{}] Checkout transaction rolled back due to domain validation failure", request_id);
        throw;
    } catch (const std::exception& e) {
        spdlog::error("[{}] Checkout transaction error: {}", request_id, e.what());
        throw exception::InternalServerException("Failed to finalize order transaction. All changes were rolled back.");
    }

    auto created_order = order_repo_->FindById(order_id, request_id);
    if (!created_order.has_value()) {
        throw exception::InternalServerException("Order committed but could not be retrieved");
    }

    return dto::OrderResponseDto::FromModel(*created_order);
}

std::vector<dto::OrderResponseDto> OrderService::GetBuyerOrders(int64_t buyer_id, const std::string& request_id) {
    auto orders = order_repo_->FindByBuyerId(buyer_id, request_id);
    std::vector<dto::OrderResponseDto> dtos;
    dtos.reserve(orders.size());
    for (const auto& order : orders) {
        dtos.push_back(dto::OrderResponseDto::FromModel(order));
    }
    return dtos;
}

dto::OrderResponseDto OrderService::GetOrderById(int64_t order_id, int64_t user_id, bool is_admin, const std::string& request_id) {
    auto order_opt = order_repo_->FindById(order_id, request_id);
    if (!order_opt.has_value()) {
        throw exception::NotFoundException("Order not found");
    }

    if (!is_admin && order_opt->buyer_id != user_id) {
        throw exception::AuthorizationException("You are not authorized to view this order");
    }

    return dto::OrderResponseDto::FromModel(*order_opt);
}

} // namespace dhivagar::dhivagarmart::service
