#pragma once

#include <cstdint>

#include "model/Money.h"
#include <string>

namespace dhivagar::dhivagarmart::util {

class MoneyUtil {
public:
    static std::string Format(model::Money money) {
        return money.ToString();
    }

    static model::Money ParseFromDollars(double dollars) {
        return model::Money::FromCents(static_cast<int64_t>(dollars * 100.0 + 0.5));
    }
};

} // namespace dhivagar::dhivagarmart::util
