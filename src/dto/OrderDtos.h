#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "model/Order.h"

namespace dhivagar::dhivagarmart::dto {

struct CreateOrderRequestDto {
    std::string shipping_address;
    std::string full_name;
    std::string phone;
    std::string city;
    std::string state;
    std::string pincode;
    std::string payment_method{"CASH_ON_DELIVERY"};
    std::string payment_nonce;
};

inline void from_json(const nlohmann::json& j, CreateOrderRequestDto& dto) {
    dto.shipping_address = j.value("shipping_address", "");
    dto.full_name = j.value("full_name", "");
    dto.phone = j.value("phone", "");
    dto.city = j.value("city", "");
    dto.state = j.value("state", "");
    dto.pincode = j.value("pincode", "");
    dto.payment_method = j.value("payment_method", "CASH_ON_DELIVERY");
    dto.payment_nonce = j.value("payment_nonce", "");
}

struct UpdateOrderStatusRequestDto {
    std::string status;
};

inline void from_json(const nlohmann::json& j, UpdateOrderStatusRequestDto& dto) {
    dto.status = j.value("status", "");
}

struct OrderItemResponseDto {
    int64_t id{0};
    int64_t product_id{0};
    std::string product_name;
    int32_t quantity{0};
    int64_t unit_price_cents{0};
    std::string unit_price_formatted;
    int64_t subtotal_cents{0};
    std::string subtotal_formatted;

    static OrderItemResponseDto FromModel(const model::OrderItem& item) {
        OrderItemResponseDto dto;
        dto.id = item.id;
        dto.product_id = item.product_id;
        dto.product_name = item.product_name;
        dto.quantity = item.quantity;
        dto.unit_price_cents = item.unit_price.GetCents();
        dto.unit_price_formatted = item.unit_price.ToString();
        dto.subtotal_cents = (item.unit_price * item.quantity).GetCents();
        dto.subtotal_formatted = (item.unit_price * item.quantity).ToString();
        return dto;
    }
};

inline void to_json(nlohmann::json& j, const OrderItemResponseDto& dto) {
    j = nlohmann::json{
        {"id", dto.id},
        {"product_id", dto.product_id},
        {"product_name", dto.product_name},
        {"quantity", dto.quantity},
        {"unit_price_cents", dto.unit_price_cents},
        {"unit_price_formatted", dto.unit_price_formatted},
        {"subtotal_cents", dto.subtotal_cents},
        {"subtotal_formatted", dto.subtotal_formatted}
    };
}

struct OrderResponseDto {
    int64_t id{0};
    int64_t buyer_id{0};
    std::string buyer_name;
    std::string buyer_email;
    std::string status;
    int64_t total_amount_cents{0};
    std::string total_amount_formatted;
    std::string payment_method{"CASH_ON_DELIVERY"};
    std::string payment_status{"PENDING"};
    std::string delivery_address;
    std::string phone;
    std::string full_name;
    std::string created_at;
    std::vector<OrderItemResponseDto> items;

    static OrderResponseDto FromModel(const model::Order& order) {
        OrderResponseDto dto;
        dto.id = order.id;
        dto.buyer_id = order.buyer_id;
        dto.buyer_name = order.buyer_name;
        dto.buyer_email = order.buyer_email;
        dto.status = model::OrderStatusToString(order.status);
        dto.total_amount_cents = order.total_amount.GetCents();
        dto.total_amount_formatted = order.total_amount.ToString();
        dto.payment_method = order.payment_method.empty() ? "CASH_ON_DELIVERY" : order.payment_method;
        dto.payment_status = order.payment_status.empty() ? "PENDING" : order.payment_status;
        dto.delivery_address = order.delivery_address;
        dto.phone = order.phone;
        dto.full_name = order.full_name;
        dto.created_at = order.created_at;
        for (const auto& item : order.items) {
            dto.items.push_back(OrderItemResponseDto::FromModel(item));
        }
        return dto;
    }
};

inline void to_json(nlohmann::json& j, const OrderResponseDto& dto) {
    j = nlohmann::json{
        {"id", dto.id},
        {"buyer_id", dto.buyer_id},
        {"buyer_name", dto.buyer_name},
        {"buyer_email", dto.buyer_email},
        {"status", dto.status},
        {"total_amount_cents", dto.total_amount_cents},
        {"total_amount_formatted", dto.total_amount_formatted},
        {"payment_method", dto.payment_method},
        {"payment_status", dto.payment_status},
        {"delivery_address", dto.delivery_address},
        {"phone", dto.phone},
        {"full_name", dto.full_name},
        {"created_at", dto.created_at},
        {"items", dto.items}
    };
}

} // namespace dhivagar::dhivagarmart::dto
