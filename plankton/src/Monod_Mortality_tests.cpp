#include "Monod.h"
#include "gtest/gtest.h"

constexpr double I_AVG_NON_LIMITING = 1000.0;

TEST(Mortality, MortalityWithNoSubstrateDecreasesBiomass) {
    // Arrange
    constexpr double kd = 0.3;
    constexpr double dt = 0.01;

    const MonodParameters params{ 100.0, 2.0, 0.5, 100, kd };
    const MonodState state{ 1.01, 0.0 };

    // Act
    const auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING, dt);

    // Assert
    EXPECT_LT(X, state.X);
}

TEST(Mortality, MortalitySlowsGrowth) {
    // Arrange
    constexpr double kd = 0.3;
    constexpr double dt = 0.01;

    const MonodParameters params_no_mortality{ 100.0, 2.0, 0.5, 100, 0.0};
    const MonodParameters params{ 100.0, 2.0, 0.5, 100, kd };
    const MonodState state{ 1.01, 5.0 };

    // Act
    const auto [X_no_mortality, S_no_mortality] = eulerStep(state, params_no_mortality, I_AVG_NON_LIMITING, dt);
    const auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING, dt);

    //Assert
    EXPECT_GT(X_no_mortality, X);
}

TEST(Mortality, NegativeKdThrowsException) {
    // Arrange
    constexpr double bad_kd = -0.1;

    // Act and Assert
    EXPECT_THROW(MonodParameters(1.0, 1.5, 6.6, 100.0, bad_kd), std::invalid_argument);
}
