#include "Monod.h"
#include "gtest/gtest.h"

TEST(Mortality, MortalityWithNoSubstrateDecreasesBiomass) {
    // Arrange
    constexpr double kd = 0.3;

    const MonodParameters params{ 100.0, 2.0, 0.5, 100, kd, 0.01 };
    constexpr MonodState state{ 1.01, 0.0 };

    // Act
    const auto [X, S] = eulerStep(state, params);

    // Assert
    EXPECT_LT(X, state.X);
}

TEST(Mortality, MortalitySlowsGrowth) {
    // Arrange
    constexpr double kd = 0.3;

    const MonodParameters params_no_mortality{ 100.0, 2.0, 0.5, 100, 0.0, 0.01 };
    const MonodParameters params{ 100.0, 2.0, 0.5, 100, kd, 0.01 };
    constexpr MonodState state{ 1.01, 5.0 };

    // Act
    const auto [X_no_mortality, S_no_mortality] = eulerStep(state, params_no_mortality);
    const auto [X, S] = eulerStep(state, params);

    //Assert
    EXPECT_GT(X_no_mortality, X);
}

TEST(Mortality, NegativeKdThrowsException) {
    // Arrange
    constexpr double bad_kd = -0.1;

    // Act and Assert
    EXPECT_THROW(MonodParameters(1.0, 1.5, 6.6, 100.0, bad_kd, 0.01), std::invalid_argument);
}
