#include "KineticParametersPanel.h"
#include "MonodParameters.h"
#include <gtest/gtest.h>

TEST(KineticParametersPanelTest, ToParametersReturnsDefaultValues) {
    KineticParametersPanel panel;
    const MonodParameters p = panel.toParameters();
    EXPECT_DOUBLE_EQ(p.Ks, 1.0);
    EXPECT_DOUBLE_EQ(p.mu_max, 1.5);
    EXPECT_DOUBLE_EQ(p.Yx_s, 6.6);
    EXPECT_DOUBLE_EQ(p.Ki, 100.0);
    EXPECT_DOUBLE_EQ(p.kd, 0.3);
}

TEST(KineticParametersPanelTest, ToParametersReturnsSetValues) {
    KineticParametersPanel panel(nullptr, 2.0, 3.0, 5.0, 50.0, 0.1);
    const MonodParameters p = panel.toParameters();
    EXPECT_DOUBLE_EQ(p.Ks, 2.0);
    EXPECT_DOUBLE_EQ(p.mu_max, 3.0);
    EXPECT_DOUBLE_EQ(p.Yx_s, 5.0);
    EXPECT_DOUBLE_EQ(p.Ki, 50.0);
    EXPECT_DOUBLE_EQ(p.kd, 0.1);
}

TEST(KineticParametersPanelTest, ToParametersReflectsUserEdits) {
    KineticParametersPanel panel;
    auto *Ks = panel.findChild<QLineEdit *>("Ks");
    auto *muMax = panel.findChild<QLineEdit *>("muMax");
    auto *Yxs = panel.findChild<QLineEdit *>("Yxs");
    auto *Ki = panel.findChild<QLineEdit *>("Ki");
    auto *kd = panel.findChild<QLineEdit *>("kd");
    ASSERT_NE(Ks, nullptr);
    ASSERT_NE(muMax, nullptr);
    ASSERT_NE(Yxs, nullptr);
    ASSERT_NE(Ki, nullptr);
    ASSERT_NE(kd, nullptr);

    Ks->setText("2.0");
    muMax->setText("3.0");
    Yxs->setText("5.0");
    Ki->setText("50.0");
    kd->setText("0.1");

    const MonodParameters p = panel.toParameters();
    EXPECT_DOUBLE_EQ(p.Ks, 2.0);
    EXPECT_DOUBLE_EQ(p.mu_max, 3.0);
    EXPECT_DOUBLE_EQ(p.Yx_s, 5.0);
    EXPECT_DOUBLE_EQ(p.Ki, 50.0);
    EXPECT_DOUBLE_EQ(p.kd, 0.1);
}

