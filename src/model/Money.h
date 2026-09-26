#pragma once

#include <cstdint>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace dhivagar::dhivagarmart::model {

/**
 * @brief Immutable value object representing monetary values in minor units (cents).
 * Prohibits floating-point rounding inaccuracies and guarantees domain invariants.
 */
class Money {
public:
    constexpr Money() noexcept : cents_(0) {}
    constexpr explicit Money(int64_t cents) noexcept : cents_(cents) {}

    [[nodiscard]] constexpr int64_t GetCents() const noexcept {
        return cents_;
    }

    [[nodiscard]] static constexpr Money FromCents(int64_t cents) noexcept {
        return Money(cents);
    }

    [[nodiscard]] static Money FromMajorUnits(int64_t dollars, int64_t cents = 0) {
        return Money(dollars * 100 + cents);
    }

    [[nodiscard]] std::string ToString() const {
        std::ostringstream oss;
        int64_t abs_cents = cents_ < 0 ? -cents_ : cents_;
        if (cents_ < 0) {
            oss << "-";
        }
        oss << "₹" << (abs_cents / 100) << "." << std::setw(2) << std::setfill('0') << (abs_cents % 100);
        return oss.str();
    }

    [[nodiscard]] double ToDouble() const noexcept {
        return static_cast<double>(cents_) / 100.0;
    }

    constexpr Money operator+(const Money& other) const noexcept {
        return Money(cents_ + other.cents_);
    }

    constexpr Money operator-(const Money& other) const noexcept {
        return Money(cents_ - other.cents_);
    }

    constexpr Money operator*(int64_t multiplier) const noexcept {
        return Money(cents_ * multiplier);
    }

    Money& operator+=(const Money& other) noexcept {
        cents_ += other.cents_;
        return *this;
    }

    Money& operator-=(const Money& other) noexcept {
        cents_ -= other.cents_;
        return *this;
    }

    constexpr auto operator<=>(const Money& other) const noexcept = default;
    constexpr bool operator==(const Money& other) const noexcept = default;

private:
    int64_t cents_;
};

} // namespace dhivagar::dhivagarmart::model
