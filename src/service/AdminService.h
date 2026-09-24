#pragma once

#include <cstdint>

#include "repository/IUserRepository.h"
#include "repository/IProductRepository.h"
#include "repository/IOrderRepository.h"
#include "dto/AuthDtos.h"
#include "dto/ProductDtos.h"
#include "dto/OrderDtos.h"
#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

namespace dhivagar::dhivagarmart::service {

struct AdminStatsDto {
    int64_t total_users{0};
    int64_t total_products{0};
    int64_t total_orders{0};
    int64_t total_revenue_cents{0};
    std::string total_revenue_formatted;
};

inline void to_json(nlohmann::json& j, const AdminStatsDto& s) {
    j = nlohmann::json{
        {"total_users", s.total_users},
        {"total_products", s.total_products},
        {"total_orders", s.total_orders},
        {"total_revenue_cents", s.total_revenue_cents},
        {"total_revenue_formatted", s.total_revenue_formatted}
    };
}

/**
 * @brief Service providing marketplace administrative controls, user directories, and statistics.
 */
class AdminService {
public:
    AdminService(std::shared_ptr<repository::IUserRepository> user_repo,
                 std::shared_ptr<repository::IProductRepository> product_repo,
                 std::shared_ptr<repository::IOrderRepository> order_repo);

    std::vector<dto::UserResponseDto> GetAllUsers(const std::string& request_id = "");
    std::vector<dto::ProductResponseDto> GetAllProducts(const std::string& request_id = "");
    void ModerateDeleteProduct(int64_t product_id, const std::string& request_id = "");
    std::vector<dto::OrderResponseDto> GetAllOrders(const std::string& request_id = "");
    dto::OrderResponseDto UpdateOrderStatus(int64_t order_id, const std::string& status_str, const std::string& request_id = "");
    AdminStatsDto GetStatistics(const std::string& request_id = "");

private:
    std::shared_ptr<repository::IUserRepository> user_repo_;
    std::shared_ptr<repository::IProductRepository> product_repo_;
    std::shared_ptr<repository::IOrderRepository> order_repo_;
};

} // namespace dhivagar::dhivagarmart::service
