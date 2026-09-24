#include <cstdint>
#include "service/AdminService.h"
#include "exception/AppException.h"
#include <spdlog/spdlog.h>

namespace dhivagar::dhivagarmart::service {

AdminService::AdminService(std::shared_ptr<repository::IUserRepository> user_repo,
                           std::shared_ptr<repository::IProductRepository> product_repo,
                           std::shared_ptr<repository::IOrderRepository> order_repo)
    : user_repo_(std::move(user_repo)),
      product_repo_(std::move(product_repo)),
      order_repo_(std::move(order_repo)) {}

std::vector<dto::UserResponseDto> AdminService::GetAllUsers(const std::string& request_id) {
    auto users = user_repo_->FindAll(request_id);
    std::vector<dto::UserResponseDto> dtos;
    dtos.reserve(users.size());
    for (const auto& u : users) {
        dtos.push_back(dto::UserResponseDto{
            .id = u.id,
            .name = u.name,
            .email = u.email,
            .role = model::UserRoleToString(u.role),
            .created_at = u.created_at
        });
    }
    return dtos;
}

std::vector<dto::ProductResponseDto> AdminService::GetAllProducts(const std::string& request_id) {
    auto products = product_repo_->FindAll(std::nullopt, std::nullopt, request_id);
    std::vector<dto::ProductResponseDto> dtos;
    dtos.reserve(products.size());
    for (const auto& p : products) {
        dtos.push_back(dto::ProductResponseDto::FromModel(p));
    }
    return dtos;
}

void AdminService::ModerateDeleteProduct(int64_t product_id, const std::string& request_id) {
    spdlog::info("[{}] AdminService::ModerateDeleteProduct product_id={}", request_id, product_id);
    if (product_id <= 0) {
        throw exception::ValidationException("Invalid product ID");
    }
    if (!product_repo_->AdminDelete(product_id, request_id)) {
        throw exception::NotFoundException("Product not found");
    }
}

std::vector<dto::OrderResponseDto> AdminService::GetAllOrders(const std::string& request_id) {
    auto orders = order_repo_->FindAll(request_id);
    std::vector<dto::OrderResponseDto> dtos;
    dtos.reserve(orders.size());
    for (const auto& o : orders) {
        dtos.push_back(dto::OrderResponseDto::FromModel(o));
    }
    return dtos;
}

dto::OrderResponseDto AdminService::UpdateOrderStatus(int64_t order_id, const std::string& status_str, const std::string& request_id) {
    spdlog::info("[{}] AdminService::UpdateOrderStatus order_id={} status={}", request_id, order_id, status_str);
    if (order_id <= 0) {
        throw exception::ValidationException("Invalid order ID");
    }

    model::OrderStatus target = model::StringToOrderStatus(status_str);
    if (!order_repo_->UpdateStatus(order_id, target, request_id)) {
        throw exception::NotFoundException("Order not found");
    }

    auto updated = order_repo_->FindById(order_id, request_id);
    return dto::OrderResponseDto::FromModel(*updated);
}

AdminStatsDto AdminService::GetStatistics(const std::string& request_id) {
    auto users = user_repo_->FindAll(request_id);
    auto products = product_repo_->FindAll(std::nullopt, std::nullopt, request_id);
    auto orders = order_repo_->FindAll(request_id);

    model::Money total_rev{model::Money::FromCents(0)};
    for (const auto& order : orders) {
        if (order.status != model::OrderStatus::kCancelled) {
            total_rev += order.total_amount;
        }
    }

    AdminStatsDto stats;
    stats.total_users = static_cast<int64_t>(users.size());
    stats.total_products = static_cast<int64_t>(products.size());
    stats.total_orders = static_cast<int64_t>(orders.size());
    stats.total_revenue_cents = total_rev.GetCents();
    stats.total_revenue_formatted = total_rev.ToString();
    return stats;
}

} // namespace dhivagar::dhivagarmart::service
