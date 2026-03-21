#include "ReactorGeometry.h"
#include <gtest/gtest.h>

TEST(ReactorGeometry, ReactorGeometryCanBeConstructed) {
    // Arrange
    const double depth = 1.0;
    const double I0 = 1.0;
    const double k = 1.0;
    // Act
    auto sut = ReactorGeometry(depth, I0, k);

    // Assert
    EXPECT_DOUBLE_EQ(sut.depth, depth);
    EXPECT_DOUBLE_EQ(sut.I0, I0);
    EXPECT_DOUBLE_EQ(sut.k, k);
}

TEST(ReactorGeometry, ReactorGeometryHasValidDepthParameter) {
    // Arrange
    const double depth = 0.0;
    const double negativeDepth = -10.0;
    const double I0 = 1.0;
    const double k = 1.0;

    // Act & Assert
    EXPECT_THROW(ReactorGeometry(depth, I0, k), std::invalid_argument);
    EXPECT_THROW(ReactorGeometry(negativeDepth, I0, k), std::invalid_argument);
}

TEST(ReactorGeometry, ReactorGeometryHasValidIrradianceParameter) {
    // Arrange
    const double depth = 1.0;
    const double I0 = -1.0;
    const double k = 1.0;

    // Act & Assert
    EXPECT_THROW(ReactorGeometry(depth, I0, k), std::invalid_argument);
}

TEST(ReactorGeometry, ReactorGeometryHasExtinctionCoefficientParameter) {
    // Arrange
    const double depth = 1.0;
    const double I0 = 1.0;
    const double zero_k = 0.0;
    const double negative_k = -1.0;

    // Act & Assert
    EXPECT_THROW(ReactorGeometry(depth, I0, zero_k), std::invalid_argument);
    EXPECT_THROW(ReactorGeometry(depth, I0, negative_k), std::invalid_argument);
}
