#include "SimulationControlsPanel.h"

#include <gtest/gtest.h>

TEST(SimulationControlsPanelTest, AccessorsReturnDefaultValues) {
    SimulationControlsPanel panel;
    EXPECT_DOUBLE_EQ(panel.dt(), 0.01);
    EXPECT_EQ(panel.numSteps(), 1000U);
}

TEST(SimulationControlsPanelTest, AccessorsReturnSetValues) {
    SimulationControlsPanel panel(nullptr, 0.05, 500U);
    EXPECT_DOUBLE_EQ(panel.dt(), 0.05);
    EXPECT_EQ(panel.numSteps(), 500U);
}

TEST(SimulationControlsPanelTest, AccessorsReflectUserEdits) {
    SimulationControlsPanel panel;
    auto* dtField = panel.findChild<QLineEdit*>("dt");
    auto* nField  = panel.findChild<QLineEdit*>("N");
    ASSERT_NE(dtField, nullptr);
    ASSERT_NE(nField,  nullptr);

    dtField->setText("0.05");
    nField->setText("500");

    EXPECT_DOUBLE_EQ(panel.dt(), 0.05);
    EXPECT_EQ(panel.numSteps(), 500U);
}
