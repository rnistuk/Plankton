#include "Monod.h"

#include <algorithm>
#include <chrono>
#include <gtest/gtest.h>

constexpr double KS = 1.0;     // Half-saturation constant (or affinity constant)
constexpr double MU_MAX = 2.0;

TEST(Monod, ZeroSubstrateReturnsZeroGrowth) {
    // Arrange
    const double S = 0.0;
    const double expected_mu = 0.0; // Specific Growth rate

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_DOUBLE_EQ(actual_mu, expected_mu);
}

TEST(Monod, PositiveSubstrateReturnsPositiveGrowth) {
    // Arrange
    const double S = 1.0; // limiting substrate concentration

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_GT(actual_mu, 0.0);
}

TEST(Monod, SubstrateEqualHalfSaturationGivesHalfMaxGrowth) {
    // Arrange
    const double S = KS; // limiting substrate concentration
    const double expected_mu = MU_MAX/2.0;

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_NEAR(actual_mu, expected_mu, 0.03);
}

TEST(Monod, HighSubstrateApproachesMaxGrowthRate) {
    // Arrange
    const double S = 100.0; // limiting substrate concentration

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_NEAR(actual_mu, MU_MAX, 0.03);
}

TEST(Monod, GrowthIncreasesWithSubstrate) {
    // Arrange
    const double S1 = 0.5;
    const double S2 = 1.5;
    const double S3 = 2.0;

    // Act
    // Assert
    EXPECT_LE(Monod(S1, KS, MU_MAX), Monod(S2, KS, MU_MAX));
    EXPECT_LE(Monod(S2, KS, MU_MAX), Monod(S3, KS, MU_MAX));
}

TEST(EulerMonad, ZeroSubstrateBiomassStaysConstant) {
    // Arrange
    MonodState state{1.0, 0.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 0.01 };

    // Act
    state = eulerStep(state, params);

    // Assert
    EXPECT_DOUBLE_EQ(state.X, 1.0);
}

TEST(EulerMonad, PositiveSubstrateIncreasesBiomass) {
    // Arrange
    const MonodState state{1.0, 5.0};
    MonodState newState{1.0, 0.0};

    const MonodParameters params{KS, MU_MAX, 0.5, 0.01 };

    // Act
    newState = eulerStep(state, params);

    // Assert
    EXPECT_GT(newState.X, state.X);
}

TEST(EulerMonad, PositiveSubstrateDecreasesSubstrate) {
    // Arrange
    const MonodState state{1.0, 5.0};
    //MonodState newState{1.0, 0.0};

    const MonodParameters params{KS, MU_MAX, 0.5, 0.01 };

    // Act
    const auto newState = eulerStep(state, params);

    // Assert
    EXPECT_LT(newState.S, state.S);
}

TEST(Stoichiometry, SubstrateConsumedEqualsBiomassProduced) {
    // Arrange
    const MonodState state{ 1.01, 5.0 };
    const MonodParameters params{ KS, MU_MAX, 0.5, 0.01 };

    // Act
    const auto newState = eulerStep(state, params);

    // Assert
    //EXPECT_DOUBLE_EQ(newState.X - state.X, (state.S-newState.S) * params.Yx_s);
    EXPECT_NEAR(newState.X - state.X, (state.S-newState.S) * params.Yx_s, 1e-9);
}

TEST(SimulateMultipleSteps, BiomassIncreasesSubstrateDecreases) {
    // Arrange
    const MonodState state{1.0, 10.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 0.1 };
    const int num_steps = 10;

    // Act
    const auto result = simulate(num_steps, state, params);


    // Assert
    EXPECT_EQ(result.size(), num_steps + 1);
    EXPECT_GT(result.back().X, state.X);
    EXPECT_LT(result.back().S, state.S);
}

TEST(SimulateMultipleSteps, SubstrateNeverNegative) {
    // Arrange
    const MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 0.1 };
    const int num_steps = 1000;

    // Act
    const auto result = simulate(num_steps, state, params);

    // Assert
    for (const auto& s : result) {
        ASSERT_GE(s.S, 0.0) << "Substrate should never be negative";
    }
}

TEST(SimulateMultipleSteps, BiomassRemainsConstantAfterSubstrateIsZero) {
    // Arrange
    const MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 0.1 };
    const int num_steps = 1000;

    //Act
    const auto results = simulate(num_steps, state, params);

    // Assert
    auto it = std::find_if(results.begin(), results.end(), [](const auto& r) {
        return r.S == 0.0;
    });

    ASSERT_NE(it, results.end()) << "No state with substrate zero found";
    auto last_biomass = it->X;
    ++it;
    while (it != results.end()) {
        ASSERT_NEAR(it->X, last_biomass, 1e-9) << "Biomass should remain constant after substrate is zero";
        last_biomass = it->X;
        ++it;
    }
}
