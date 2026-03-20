#include "ReactorGeometry.h"
#include <gtest/gtest.h>

TEST(ReactorGeometry, ReactorGeometryExists) {
    // Arrange
    // Act
    auto sut = ReactorGeometry();

    // Assert
    EXPECT_TRUE(&sut);
}