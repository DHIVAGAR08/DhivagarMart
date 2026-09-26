#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "model/Money.h"

namespace dhivagar::dhivagarmart::model {

enum class OrderStatus {
    kPending,
    kConfirmed,
    kProcessing,
    kShipped,
    kDelivered,
    kCancelled
};

inline std::string OrderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::kPending: return "PENDING";
        case OrderStatus::kConfirmed: return "CONFIRMED";
        case OrderStatus::kProcessing: return "PROCESSING";
        case OrderStatus::kShipped: return "SHIPPED";
        case OrderStatus::kDelivered: return "DELIVERED";
        case OrderStatus::kCancelled: return "CANCELLED";
        default: return "PENDING";
    }
}

inline OrderStatus StringToOrderStatus(const std::string& status_str) {
    if (status_str == "CONFIRMED") return OrderStatus::kConfirmed;
    if (status_str == "PROCESSING") return OrderStatus::kProcessing;
    if (status_str == "SHIPPED") return OrderStatus::kShipped;
    if (status_str == "DELIVERED") return OrderStatus::kDelivered;
    if (status_str == "CANCELLED") return OrderStatus::kCancelled;
    return OrderStatus::kPending;
}

struct OrderItem {
    int64_t id{0};
    int64_t order_id{0};
    int64_t product_id{0};
    std::string product_name;
    int32_t quantity{0};
    Money unit_price{Money::FromCents(0)};
    std::string created_at;
};

struct Order {
    int64_t id{0};
    int64_t buyer_id{0};
    std::string buyer_name;
    std::string buyer_email;
    OrderStatus status{OrderStatus::kPending};
    Money total_amount{Money::FromCents(0)};
    std::string payment_method{"CASH_ON_DELIVERY"};
    std::string payment_status{"PENDING"};
    std::string delivery_address;
    std::string phone;
    std::string full_name;
    std::string created_at;
    std::vector<OrderItem> items;
};

} // namespace dhivagar::dhivagarmart::model
