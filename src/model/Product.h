#pragma once

#include <string>
#include <cstdint>
#include "model/Money.h"

namespace dhivagar::dhivagarmart::model {

struct Product {
    int64_t id{0};
    int64_t seller_id{0};
    std::string name;
    std::string description;
    Money price{Money::FromCents(0)};
    int32_t stock_qty{0};
    std::string category;
    std::string image_url;
    std::string created_at;
};

} // namespace dhivagar::dhivagarmart::model
