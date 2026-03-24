#include "ReactorGeometry.h"
#include "BeerLambert.h"
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
    const double zeroI0 = 0.0;
    const double k = 1.0;

    // Act & Assert
    EXPECT_THROW(ReactorGeometry(depth, I0, k), std::invalid_argument);
    EXPECT_THROW(ReactorGeometry(depth, zeroI0, k), std::invalid_argument);
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

TEST(ReactorGeometry, BeerLambertNoAttenuationAtSurface) {
    // Arrange
    const double reactorDepth = 1.0;
    const double I0 = 1.0;
    const double k = 1.0;
    const ReactorGeometry sut(reactorDepth, I0, k);
    const double X = 100.0; // ug C/Ml
    const double z = 0.0;

    // Act
    auto actual = beerLambert(z, sut, X);

    // Assert
    EXPECT_DOUBLE_EQ(actual, 1.0);
}

TEST(ReactorGeometry, BeerLambertIrradianceDecreasesWithDepth) {
    // Arrange
    const double maxReactorDepth = 10.0;
    const double I0 = 1.0;
    const double k = 1.0;
    const double X = 1.0; // ug C/Ml
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
    const double reactorDepth = 1.0;
    const double I0 = 1.0;
    const double k = 1.0;
    const ReactorGeometry sut(reactorDepth, I0, k);
    const double X1 = 50.0; // ug C/Ml
    const double X2 = 100.0; // ug C/Ml
    double z = reactorDepth / 2.0;

    // Act
    const auto irradiance1 = beerLambert(z, sut, X1);
    const auto irradiance2 = beerLambert(z, sut, X2);

    // Assert
    EXPECT_LT(irradiance2, irradiance1);
}

TEST(ReactorGeometry, BeerLambertDepthAveragedIrradianceFitsKnownAnalyticalValue) {
    // Arrange
    const double reactorDepth = 1.0;
    const double I0 = 100.0;
    const double k = 1.0;
    const double X = 1.0;
    const double expected_I_ave = (100 / (1.0 * 1.0 * 1.0)) * (1 - exp(-1.0));
    const auto sut = ReactorGeometry(reactorDepth, I0, k);

    // Act
    const auto actual = depthAveragedIrradiance(sut, X);

    // Assert
    ASSERT_NEAR(actual, expected_I_ave, 0.00001);
}

TEST(ReactorGeometry, BeerLambertDepthAveragedIrradianceDecreasesWithIncreasingBiomass) {
    // Arrange
    const double reactorDepth = 1.0;
    const double I0 = 100.0;
    const double k = 1.0;
    const double X0 = 1.0;
    const double X1 = 100.0;
    const auto sut = ReactorGeometry(reactorDepth, I0, k);

    // Act
    const auto I_ave_0 = depthAveragedIrradiance(sut, X0);
    const auto I_ave_1 = depthAveragedIrradiance(sut, X1);

    // Assert
    ASSERT_LT(I_ave_1, I_ave_0);
}

TEST(ReactorGeometry, BeerLambertDepthAveragedIrradianceDecreasesWithIncreasingDepth) {
    // Arrange
    const double I0 = 100.0;
    const double k = 1.0;
    const double X = 1.0;
    const double reactorDepth0 = 0.25;
    const double reactorDepth1 = 0.75;
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
    const double reactorDepth = 2.0;
    const double I0 = 100.0;
    const double k = 1.0;
    const double X = 0.0;
    const auto sut = ReactorGeometry(reactorDepth, I0, k);

    // Act
    const auto I_ave_0 = depthAveragedIrradiance(sut, X);

    // Assert
    ASSERT_DOUBLE_EQ(I_ave_0, I0);
}
