#include "ReactorGeometryPanel.h"
#include <gtest/gtest.h>

TEST(ReactorGeometryPanelTest, ToGeometryReturnsDefaultValues) {
    ReactorGeometryPanel panel;
    const ReactorGeometry geom = panel.toGeometry();
    EXPECT_DOUBLE_EQ(geom.depth, 0.05);
    EXPECT_DOUBLE_EQ(geom.I0, 200.0);
    EXPECT_DOUBLE_EQ(geom.k, 0.2);
}

TEST(ReactorGeometryPanelTest, ToGeometryReturnsCustomValues) {
    double depth = 0.1;
    double I0 = 300.0;
    double k = 0.3;
    ReactorGeometryPanel panel(nullptr,
        depth, I0, k);
    const ReactorGeometry geom = panel.toGeometry();
    EXPECT_DOUBLE_EQ(geom.depth, depth);
    EXPECT_DOUBLE_EQ(geom.I0, I0);
    EXPECT_DOUBLE_EQ(geom.k, k);
}

TEST(ReactorGeometryPanelTest, ToStateReflectsUserEdits) {
    // Arrange
    ReactorGeometryPanel panel;

    // Act
    auto* depthField = panel.findChild<QLineEdit*>("depth");
    auto* I0Field = panel.findChild<QLineEdit*>("I0");
    auto* kField = panel.findChild<QLineEdit*>("k");

    depthField->setText("0.1");
    I0Field->setText("300.0");
    kField->setText("0.3");

    // Assert
    const ReactorGeometry geom = panel.toGeometry();
    EXPECT_DOUBLE_EQ(geom.depth, 0.1);
    EXPECT_DOUBLE_EQ(geom.I0, 300.0);
    EXPECT_DOUBLE_EQ(geom.k, 0.3);
}
