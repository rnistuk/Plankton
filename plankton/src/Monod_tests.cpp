#include "Monod.h"
#include "SimulationParameters.h"
#include "BeerLambert.h"
#include "Simulation.h"

#include <algorithm>
#include <gtest/gtest.h>

constexpr double KS = 1.0;     // Half-saturation constant (or affinity constant)
constexpr double MU_MAX = 2.0;
constexpr double I_AVG_NON_LIMITING = 1000.0;

TEST(EulerMonod, ZeroSubstrateBiomassStaysConstant) {
    // Arrange
    MonodState state{1.0, 0.0};
    constexpr double dt = 0.01;
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0};

    // Act
    state = eulerStep(state, params, I_AVG_NON_LIMITING, dt);

    // Assert
    EXPECT_DOUBLE_EQ(state.X, 1.0);
}

TEST(EulerMonod, PositiveSubstrateIncreasesBiomass) {
    // Arrange
    const MonodState state{1.0, 5.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100, 0.0};
    constexpr double dt = 0.01;

    // Act
    auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING, dt);

    // Assert
    EXPECT_GT(X, state.X);
}

TEST(EulerMonod, PositiveSubstrateDecreasesSubstrate) {
    // Arrange
    constexpr double dt = 0.01;
    const MonodState state{1.0, 5.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100, 0.0 };

    // Act
    const auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING, dt);

    // Assert
    EXPECT_LT(S, state.S);
}

TEST(EulerMonod, HighLightHighSubstrateGivesFullGrowth) {
    // Arrange
    constexpr double dt = 0.01;
    const MonodState state{1.0, 100.0};   // high substrate
    constexpr double Ki = 50.0;
    const MonodParameters params{KS, MU_MAX, 0.5, Ki, 0.0};
    constexpr double I_avg = 1000.0;          // >> Ki, light is non-limiting

    // Act
    const auto [X, S] = eulerStep(state, params, I_avg, dt);

    // Assert
    EXPECT_GT(X, state.X);
}

TEST(LightLimitedGrowthRate, ZeroLightReturnsZeroGrowthRate) {
    // Arrange
    constexpr double S = 5.0;
    constexpr double Ki = 50.0;
    const MonodParameters params{KS, MU_MAX, 0.5, Ki, 0.0};
    constexpr double I_avg = 0.0;

    // Act
    const auto mu = lightLimitedGrowthRate(S, I_avg, params);

    // Assert
    EXPECT_DOUBLE_EQ(mu, 0.0);

}

TEST(LightLimitedGrowthRate, LightAtHalfSaturationGivesHalfMaxGrowth) {
    // Arrange
    constexpr double S = KS;
    constexpr double Ki = 50.0;
    const MonodParameters params{KS, MU_MAX, 0.5, Ki, 0.0};
    constexpr double I_avg = Ki;

    // Act
    const auto mu = lightLimitedGrowthRate(S, I_avg, params);

    // Assert
    EXPECT_NEAR(mu, MU_MAX/2.0, 0.03);
}

TEST(Stoichiometry, SubstrateConsumedEqualsBiomassProduced) {
    // Arrange
    constexpr double dt = 0.01;
    const MonodState state{ 1.01, 5.0 };
    const MonodParameters params{ KS, MU_MAX, 0.5, 100, 0.0};

    // Act
    const auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING, dt);

    // Assert
    //EXPECT_DOUBLE_EQ(newState.X - state.X, (state.S-newState.S) * params.Yx_s);
    EXPECT_NEAR(X - state.X, (state.S-S) * params.Yx_s, 1e-9);
}

TEST(ParameterValidation, NegativeKsThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodParameters(-1.0, 1.5, 6.6, 100.0, 0.0), std::invalid_argument);
}

TEST(ParameterValidation, Negativemu_maxThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodParameters(1.0, -1.5, 6.6, 100.0, 0.0), std::invalid_argument);
}

TEST(ParameterValidation, NegativeYx_sThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodParameters(1.0, 1.5, -6.6, 100.0, 0.0), std::invalid_argument);
}

TEST(ParameterValidation, NegativeTimeStepThrowsException) {
    // Arrange
    const MonodParameters params{1.0, 1.5, 6.6, 100.0, 0.0};
    const ReactorGeometry geometry{0.05, 200.0, 0.2};
    // Act & Assert
    EXPECT_THROW(SimulationParameters(params, geometry, -0.1, 1000), std::invalid_argument);
}

TEST(StateValidation, NegativeBiomassThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW( MonodState(-1.0, 5.0), std::invalid_argument);
}

TEST(StateValidation, NegativeSubstrateThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodState(50.0, -5.0), std::invalid_argument);
}

TEST(ParameterValidation, NegativeKiThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW( MonodParameters(1.0, 1.5, 6.6, -0.1, 0.0), std::invalid_argument);
}

TEST(ParameterValidation, ZeroKiThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW( MonodParameters(1.0, 1.5, 6.6, 0.0, 0.0), std::invalid_argument);
}
