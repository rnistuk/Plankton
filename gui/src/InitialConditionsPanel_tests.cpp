#include "InitialConditionsPanel.h"
#include "MonodState.h"

#include <gtest/gtest.h>

TEST(InitialConditionsPanelTest, ToStateReturnsDefaultValues) {
    // Arrange
    InitialConditionsPanel panel;

    // Act
    const MonodState state = panel.toState();

    // Assert
    EXPECT_DOUBLE_EQ(state.X, 2.0);
    EXPECT_DOUBLE_EQ(state.S, 2.0);
}

TEST(InitialConditionsPanelTest, ToStateReturnsSetValues) {
    // Arrange
    const double initialBiomass = 3.5;
    const double initialSubstrate = 7.1;
    InitialConditionsPanel panel(nullptr,initialBiomass,initialSubstrate);

    // Act
    const MonodState state = panel.toState();

    // Assert
    EXPECT_DOUBLE_EQ(state.X, initialBiomass);
    EXPECT_DOUBLE_EQ(state.S, initialSubstrate);
}

TEST(InitialConditionsPanelTest, ToStateReflectsUserEdits) {
    // Arrange
    InitialConditionsPanel panel;
    auto* xField = panel.findChild<QLineEdit*>("X");
    auto* sField = panel.findChild<QLineEdit*>("S");
    ASSERT_NE(xField, nullptr);
    ASSERT_NE(sField, nullptr);
    xField->setText("5.5");
    sField->setText("3.25");
    // Act
    const MonodState state = panel.toState();

    // Assert
    EXPECT_DOUBLE_EQ(state.X, 5.5);
    EXPECT_DOUBLE_EQ(state.S, 3.25);
}

