#include <gtest/gtest.h>
#include "model/Money.h"

using namespace dhivagar::dhivagarmart::model;

TEST(MoneyTest, DefaultConstructorZeroCents) {
    Money m;
    EXPECT_EQ(m.GetCents(), 0);
    EXPECT_EQ(m.ToString(), "$0.00");
}

TEST(MoneyTest, FromCentsAndMajorUnits) {
    Money m1 = Money::FromCents(499);
    EXPECT_EQ(m1.GetCents(), 499);
    EXPECT_EQ(m1.ToString(), "$4.99");

    Money m2 = Money::FromMajorUnits(15, 50);
    EXPECT_EQ(m2.GetCents(), 1550);
    EXPECT_EQ(m2.ToString(), "$15.50");
}

TEST(MoneyTest, ArithmeticOperations) {
    Money m1 = Money::FromCents(1050);
    Money m2 = Money::FromCents(425);

    Money sum = m1 + m2;
    EXPECT_EQ(sum.GetCents(), 1475);
    EXPECT_EQ(sum.ToString(), "$14.75");

    Money diff = m1 - m2;
    EXPECT_EQ(diff.GetCents(), 625);
    EXPECT_EQ(diff.ToString(), "$6.25");

    Money mult = m2 * 3;
    EXPECT_EQ(mult.GetCents(), 1275);
    EXPECT_EQ(mult.ToString(), "$12.75");
}

TEST(MoneyTest, Comparisons) {
    Money a = Money::FromCents(100);
    Money b = Money::FromCents(200);
    Money c = Money::FromCents(100);

    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_EQ(a, c);
    EXPECT_LE(a, c);
    EXPECT_GE(b, a);
}
