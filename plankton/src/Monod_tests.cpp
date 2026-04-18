#include "Monod.h"
#include "SimulationParameters.h"

#include <algorithm>
#include <gtest/gtest.h>

#include "Simulation.h"

constexpr double KS = 1.0;     // Half-saturation constant (or affinity constant)
constexpr double MU_MAX = 2.0;
constexpr double I_AVG_NON_LIMITING = 1000.0;

TEST(Monod, ZeroSubstrateReturnsZeroGrowth) {
    // Arrange
    constexpr double S = 0.0;
    constexpr double expected_mu = 0.0; // Specific Growth rate

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_DOUBLE_EQ(actual_mu, expected_mu);
}

TEST(Monod, PositiveSubstrateReturnsPositiveGrowth) {
    // Arrange
    constexpr double S = 1.0; // limiting substrate concentration

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_GT(actual_mu, 0.0);
}

TEST(Monod, SubstrateEqualHalfSaturationGivesHalfMaxGrowth) {
    // Arrange
    constexpr double S = KS; // limiting substrate concentration
    constexpr double expected_mu = MU_MAX/2.0;

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_NEAR(actual_mu, expected_mu, 0.03);
}

TEST(Monod, HighSubstrateApproachesMaxGrowthRate) {
    // Arrange
    constexpr double S = 100.0; // limiting substrate concentration

    // Act
    const auto actual_mu = Monod(S, KS, MU_MAX);

    // Assert
    EXPECT_NEAR(actual_mu, MU_MAX, 0.03);
}

TEST(Monod, GrowthIncreasesWithSubstrate) {
    // Arrange
    constexpr double S1 = 0.5;
    constexpr double S2 = 1.5;
    constexpr double S3 = 2.0;

    // Act
    // Assert
    EXPECT_LE(Monod(S1, KS, MU_MAX), Monod(S2, KS, MU_MAX));
    EXPECT_LE(Monod(S2, KS, MU_MAX), Monod(S3, KS, MU_MAX));
}

TEST(EulerMonod, ZeroSubstrateBiomassStaysConstant) {
    // Arrange
    MonodState state{1.0, 0.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0, 0.01 };

    // Act
    state = eulerStep(state, params, I_AVG_NON_LIMITING);

    // Assert
    EXPECT_DOUBLE_EQ(state.X, 1.0);
}

TEST(EulerMonod, PositiveSubstrateIncreasesBiomass) {
    // Arrange
    constexpr MonodState state{1.0, 5.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100, 0.0, 0.01 };

    // Act
    auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING);

    // Assert
    EXPECT_GT(X, state.X);
}

TEST(EulerMonod, PositiveSubstrateDecreasesSubstrate) {
    // Arrange
    constexpr MonodState state{1.0, 5.0};

    const MonodParameters params{KS, MU_MAX, 0.5, 100, 0.0, 0.01 };

    // Act
    const auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING);

    // Assert
    EXPECT_LT(S, state.S);
}

TEST(EulerMonod, HighLightHighSubstrateGivesFullGrowth) {
    // Arrange
    constexpr MonodState state{1.0, 100.0};   // high substrate
    constexpr double Ki = 50.0;
    const MonodParameters params{KS, MU_MAX, 0.5, Ki, 0.0, 0.01};
    constexpr double I_avg = 1000.0;          // >> Ki, light is non-limiting

    // Act
    const auto [X, S] = eulerStep(state, params, I_avg);

    // Assert
    EXPECT_GT(X, state.X);
}

TEST(LightLimitedGrowthRate, ZeroLightReturnsZeroGrowthRate) {
    // Arrange
    constexpr double S = 5.0;
    constexpr double Ki = 50.0;
    const MonodParameters params{KS, MU_MAX, 0.5, Ki, 0.0, 0.01};
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
    const MonodParameters params{KS, MU_MAX, 0.5, Ki, 0.0, 0.01};
    constexpr double I_avg = Ki;

    // Act
    const auto mu = lightLimitedGrowthRate(S, I_avg, params);

    // Assert
    EXPECT_NEAR(mu, MU_MAX/2.0, 0.03);
}

TEST(LightLimitedGrowthRate, SubstrateLimitingOverridesLight) {
    // Arrange
    constexpr double S = 19.0; // relatively low value for substrate.
    constexpr double Ki = 50.0;
    const MonodParameters params{KS, MU_MAX, 0.5, Ki, 0.0, 0.01};
    constexpr double I_avg = 1000.0; // A really high light intensity
    const auto expected_mu = Monod(S, KS, MU_MAX);

    // Act
    const auto mu = lightLimitedGrowthRate(S, I_avg, params);

    // Assert "when light is non-limiting, the light-coupled rate equals the
    // substrate-only Monod rate".
    EXPECT_NEAR(expected_mu, mu, 0.03);
}

TEST(Stoichiometry, SubstrateConsumedEqualsBiomassProduced) {
    // Arrange
    constexpr MonodState state{ 1.01, 5.0 };
    const MonodParameters params{ KS, MU_MAX, 0.5, 100, 0.0, 0.01 };

    // Act
    const auto [X, S] = eulerStep(state, params, I_AVG_NON_LIMITING);

    // Assert
    //EXPECT_DOUBLE_EQ(newState.X - state.X, (state.S-newState.S) * params.Yx_s);
    EXPECT_NEAR(X - state.X, (state.S-S) * params.Yx_s, 1e-9);
}

TEST(SimulateMultipleSteps, BiomassIncreasesSubstrateDecreases) {
    // Arrange
    constexpr MonodState state{1.0, 10.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0, 0.1 };
    constexpr int num_steps = 10;
    // TODO: the reactor geometry values are hard coded for now.
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry);

    // Act
    const auto result = simulate(num_steps, state, simParams);


    // Assert
    EXPECT_EQ(result.size(), num_steps + 1);
    EXPECT_GT(result.back().X, state.X);
    EXPECT_LT(result.back().S, state.S);
}

TEST(SimulateMultipleSteps, SubstrateNeverNegative) {
    // Arrange
    constexpr MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0, 0.1 };
    constexpr int num_steps = 1000;
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry);

    // Act & Assert
    for (const auto&[X, S, I] : simulate(num_steps, state, simParams)) {
        ASSERT_GE(S, 0.0) << "Substrate should never be negative";
    }
}

TEST(SimulateMultipleSteps, CantForceNegativeBiomassWithHighKd) {
    constexpr MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 5.0, 0.1 };
    constexpr int num_steps = 1000;
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry);

    // Act & Assert
    for (const auto&[X, S, I] : simulate(num_steps, state, simParams)) {
        ASSERT_GE(X, 0.0) << "Biomass should never be negative";
    }
}

TEST(SimulateMultipleSteps, BiomassRemainsConstantAfterSubstrateIsZero) {
    // Arrange
    constexpr MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0,  0.1 };
    constexpr int num_steps = 1000;  // TODO: the reactor geometry values are hard coded for now.
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry);

    //Act
    const auto results = simulate(num_steps, state, simParams);

    // Assert
    auto it = std::ranges::find_if(results.begin(), results.end(), [](const auto& r) {
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

TEST(SimulateMultipleSteps, DeeperReactorReducesGrowth) {
    constexpr MonodState state{1.0, 10.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0, 0.1};
    constexpr int num_steps = 10;

    const ReactorGeometry shallow{0.01, 200.0, 0.2};  // 1 cm
    const ReactorGeometry deep{0.20, 200.0, 0.2};     // 20 cm
    const auto simParamsShallow = SimulationParameters(params, shallow);
    const auto simParamsDeep = SimulationParameters(params, deep);

    const auto shallow_result = simulate(num_steps, state, simParamsShallow);
    const auto deep_result    = simulate(num_steps, state, simParamsDeep);

    EXPECT_GT(shallow_result.back().X, deep_result.back().X);
}

TEST(SimulateMultipleSteps, ResultContainsIAvg) {
    // Arrange
    constexpr MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0, 0.1 };
    constexpr int num_steps = 1000;
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry);

    // Act
    auto results = simulate(num_steps, state, simParams);

    // Assert
    auto [X, S, Iavg] = results.front();
    EXPECT_NEAR(Iavg, 157.4, 0.1);
}

TEST(ParameterValidation, NegativeKsThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodParameters(-1.0, 1.5, 6.6, 100.0, 0.0, 0.1), std::invalid_argument);
}

TEST(ParameterValidation, Negativemu_maxThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodParameters(1.0, -1.5, 6.6, 100.0, 0.0, 0.1), std::invalid_argument);
}

TEST(ParameterValidation, NegativeYx_sThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodParameters(1.0, 1.5, -6.6, 100.0, 0.0, 0.1), std::invalid_argument);
}

TEST(ParameterValidation, NegativeTimeStepThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW(MonodParameters(1.0, 1.5, 6.6, 100.0, 0.0, -0.1), std::invalid_argument);
}

TEST(StateValidation, NegativeBiomassThrowsException) {
    // Arrange
    constexpr MonodState state{-1.0, 5.0};  // Negative biomass
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0, 0.1};
    double depth = 0.05; // 5 cm
    double I0 = 200.0; // moderate sunlight
    double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry);

    // Act & Assert
    EXPECT_THROW(simulate(10, state, simParams), std::invalid_argument);
}

TEST(StateValidation, NegativeSubstrateThrowsException) {
    // Arrange
    constexpr MonodState state{50.0, -5.0};  // Negative substrate
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0, 0.1};
    double depth = 0.05; // 5 cm
    double I0 = 200.0; // moderate sunlight
    double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry);

    // Act & Assert
    EXPECT_THROW(simulate(10, state, simParams), std::invalid_argument);
}

TEST(ParameterValidation, NegativeKiThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW( MonodParameters(1.0, 1.5, 6.6, -0.1, 0.0, 0.1), std::invalid_argument);
}

TEST(ParameterValidation, ZeroKiThrowsException) {
    // Arrange, Act & Assert
    EXPECT_THROW( MonodParameters(1.0, 1.5, 6.6, 0.0, 0.0, 0.1), std::invalid_argument);
}
