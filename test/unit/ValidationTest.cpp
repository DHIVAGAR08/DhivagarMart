#include <gtest/gtest.h>
#include "util/ValidationUtil.h"
#include "util/PasswordUtil.h"

using namespace dhivagar::dhivagarmart::util;

TEST(ValidationTest, EmailValidation) {
    EXPECT_TRUE(ValidationUtil::IsValidEmail("test@example.com"));
    EXPECT_TRUE(ValidationUtil::IsValidEmail("user.name+tag@domain.co.in"));
    EXPECT_FALSE(ValidationUtil::IsValidEmail(""));
    EXPECT_FALSE(ValidationUtil::IsValidEmail("invalid_email"));
    EXPECT_FALSE(ValidationUtil::IsValidEmail("@domain.com"));
    EXPECT_FALSE(ValidationUtil::IsValidEmail("user@.com"));
}

TEST(ValidationTest, PasswordValidation) {
    EXPECT_TRUE(ValidationUtil::IsValidPassword("123456"));
    EXPECT_TRUE(ValidationUtil::IsValidPassword("SecurePassword!2026"));
    EXPECT_FALSE(ValidationUtil::IsValidPassword("12345"));
    EXPECT_FALSE(ValidationUtil::IsValidPassword(""));
}

TEST(ValidationTest, RoleValidation) {
    EXPECT_TRUE(ValidationUtil::IsValidRole("BUYER"));
    EXPECT_TRUE(ValidationUtil::IsValidRole("SELLER"));
    EXPECT_FALSE(ValidationUtil::IsValidRole("ADMIN")); // Admin registration strictly disallowed
    EXPECT_FALSE(ValidationUtil::IsValidRole("GUEST"));
    EXPECT_FALSE(ValidationUtil::IsValidRole(""));
}

TEST(ValidationTest, RatingValidation) {
    EXPECT_TRUE(ValidationUtil::IsValidRating(1));
    EXPECT_TRUE(ValidationUtil::IsValidRating(3));
    EXPECT_TRUE(ValidationUtil::IsValidRating(5));
    EXPECT_FALSE(ValidationUtil::IsValidRating(0));
    EXPECT_FALSE(ValidationUtil::IsValidRating(6));
    EXPECT_FALSE(ValidationUtil::IsValidRating(-1));
}

TEST(ValidationTest, HtmlEscaping) {
    std::string input = "<script>alert('xss & attack');</script>";
    std::string escaped = ValidationUtil::EscapeHtml(input);
    EXPECT_EQ(escaped.find("<script>"), std::string::npos);
    EXPECT_NE(escaped.find("&lt;script&gt;"), std::string::npos);
    EXPECT_NE(escaped.find("&amp;"), std::string::npos);
    EXPECT_NE(escaped.find("&#x27;"), std::string::npos);
}
