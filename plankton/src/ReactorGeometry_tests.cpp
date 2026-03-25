#include "ReactorGeometry.h"
#include "BeerLambert.h"
#include <gtest/gtest.h>

TEST(ReactorGeometry, ReactorGeometryCanBeConstructed) {
    // Arrange
    constexpr double depth = 1.0;
    constexpr double I0 = 1.0;
    constexpr double k = 1.0;
    // Act
    const auto sut = ReactorGeometry(depth, I0, k);

    // Assert
    EXPECT_DOUBLE_EQ(sut.depth, depth);
    EXPECT_DOUBLE_EQ(sut.I0, I0);
    EXPECT_DOUBLE_EQ(sut.k, k);
}

TEST(ReactorGeometry, ReactorGeometryHasValidDepthParameter) {
    // Arrange
    constexpr double depth = 0.0;
    constexpr double negativeDepth = -10.0;
    constexpr double I0 = 1.0;
    constexpr double k = 1.0;

    // Act & Assert
    EXPECT_THROW(ReactorGeometry(depth, I0, k), std::invalid_argument);
    EXPECT_THROW(ReactorGeometry(negativeDepth, I0, k), std::invalid_argument);
}

TEST(ReactorGeometry, ReactorGeometryHasValidIrradianceParameter) {
    // Arrange
    constexpr double depth = 1.0;
    constexpr double I0 = -1.0;
    constexpr double zeroI0 = 0.0;
    constexpr double k = 1.0;

    // Act & Assert
    EXPECT_THROW(ReactorGeometry(depth, I0, k), std::invalid_argument);
    EXPECT_THROW(ReactorGeometry(depth, zeroI0, k), std::invalid_argument);
}

TEST(ReactorGeometry, ReactorGeometryHasExtinctionCoefficientParameter) {
    // Arrange
    constexpr double depth = 1.0;
    constexpr double I0 = 1.0;
    constexpr double zero_k = 0.0;
    constexpr double negative_k = -1.0;

    // Act & Assert
    EXPECT_THROW(ReactorGeometry(depth, I0, zero_k), std::invalid_argument);
    EXPECT_THROW(ReactorGeometry(depth, I0, negative_k), std::invalid_argument);
}

TEST(ReactorGeometry, BeerLambertNoAttenuationAtSurface) {
    // Arrange
    constexpr double reactorDepth = 1.0;
    constexpr double I0 = 1.0;
    constexpr double k = 1.0;
    const ReactorGeometry sut(reactorDepth, I0, k);
    constexpr double X = 100.0; // ug C/Ml
    constexpr double z = 0.0;

    // Act
    const auto actual = beerLambert(z, sut, X);

    // Assert
    EXPECT_DOUBLE_EQ(actual, 1.0);
}

TEST(ReactorGeometry, BeerLambertIrradianceDecreasesWithDepth) {
    // Arrange
    constexpr double maxReactorDepth = 10.0;
    constexpr double I0 = 1.0;
    constexpr double k = 1.0;
    constexpr double X = 1.0; // ug C/Ml
    const ReactorGeometry sut(maxReactorDepth, I0, k);
    double z = 0.0;
    auto lastIrradiance = beerLambert(z, sut, X);

    // Act and Assert
    while (z < maxReactorDepth) {
        z += 0.1;
        auto actual = beerLambert(z, sut, X);
        EXPECT_LT(actual, lastIrradiance);
        lastIrradiance = actual;
    }
}

TEST(ReactorGeometry, BeerLambertIrradianceDecreasesWithIncreaseingBiomass) {
    // Arrange
    constexpr double reactorDepth = 1.0;
    constexpr double I0 = 1.0;
    constexpr double k = 1.0;
    const ReactorGeometry sut(reactorDepth, I0, k);
    constexpr double X1 = 50.0; // ug C/Ml
    constexpr double X2 = 100.0; // ug C/Ml
    constexpr double z = reactorDepth / 2.0;

    // Act
    const auto irradiance1 = beerLambert(z, sut, X1);
    const auto irradiance2 = beerLambert(z, sut, X2);

    // Assert
    EXPECT_LT(irradiance2, irradiance1);
}

TEST(ReactorGeometry, BeerLambertDepthAveragedIrradianceFitsKnownAnalyticalValue) {
    // Arrange
    constexpr double reactorDepth = 1.0;
    constexpr double I0 = 100.0;
    constexpr double k = 1.0;
    constexpr double X = 1.0;
    const double expected_I_ave = 100 * (1 - exp(-1.0)) / (1.0 * 1.0 * 1.0);
    const auto sut = ReactorGeometry(reactorDepth, I0, k);

    // Act
    const auto actual = depthAveragedIrradiance(sut, X);

    // Assert
    ASSERT_NEAR(actual, expected_I_ave, 0.00001);
}

TEST(ReactorGeometry, BeerLambertDepthAveragedIrradianceDecreasesWithIncreasingBiomass) {
    // Arrange
    constexpr double reactorDepth = 1.0;
    constexpr double I0 = 100.0;
    constexpr double k = 1.0;
    constexpr double X0 = 1.0;
    constexpr double X1 = 100.0;
    const auto sut = ReactorGeometry(reactorDepth, I0, k);

    // Act
    const auto I_ave_0 = depthAveragedIrradiance(sut, X0);
    const auto I_ave_1 = depthAveragedIrradiance(sut, X1);

    // Assert
    ASSERT_LT(I_ave_1, I_ave_0);
}

TEST(ReactorGeometry, BeerLambertDepthAveragedIrradianceDecreasesWithIncreasingDepth) {
    // Arrange
    constexpr double I0 = 100.0;
    constexpr double k = 1.0;
    constexpr double X = 1.0;
    constexpr double reactorDepth0 = 0.25;
    constexpr double reactorDepth1 = 0.75;
    const auto sut0 = ReactorGeometry(reactorDepth0, I0, k);
    const auto sut1 = ReactorGeometry(reactorDepth1, I0, k);

    // Act
    const auto I_ave_0 = depthAveragedIrradiance(sut0, X);
    const auto I_ave_1 = depthAveragedIrradiance(sut1, X);

    // Assert
    ASSERT_LT(I_ave_1, I_ave_0);
}

TEST(ReactorGeometry, BeerLambertDepthAveragedIrradianceReturnsI0IfZeroBiomass) {
    // Arrange
    constexpr double reactorDepth = 2.0;
    constexpr double I0 = 100.0;
    constexpr double k = 1.0;
    constexpr double X = 0.0;
    const auto sut = ReactorGeometry(reactorDepth, I0, k);

    // Act
    const auto I_ave_0 = depthAveragedIrradiance(sut, X);

    // Assert
    ASSERT_DOUBLE_EQ(I_ave_0, I0);
}
