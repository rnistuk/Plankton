#include "Monod.h"
#include "SimulationParameters.h"
#include "BeerLambert.h"
#include "Simulation.h"

#include <algorithm>
#include <gtest/gtest.h>

constexpr double KS = 1.0; // Half-saturation constant (or affinity constant)
constexpr double MU_MAX = 2.0;

TEST(SimulateMultipleSteps, BiomassIncreasesSubstrateDecreases) {
    // Arrange
    constexpr double dt = 0.01;
    const MonodState state{1.0, 10.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0};
    constexpr int num_steps = 10;
    // TODO: the reactor geometry values are hard coded for now.
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    bool stop = false;
    const auto simParams = SimulationParameters(params, geometry, dt, num_steps);
    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };

    // Act
    const auto result = simulate(state, simParams
                                 , lightModel, stop);

    // Assert
    EXPECT_EQ(result.size(), num_steps + 1);
    EXPECT_GT(result.back().X, state.X);
    EXPECT_LT(result.back().S, state.S);
}

TEST(SimulateMultipleSteps, SubstrateNeverNegative) {
    // Arrange
    constexpr double dt = 0.01;
    const MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0};
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    constexpr size_t numSteps = 1000;
    bool stop = false;
    const auto simParams = SimulationParameters(params, geometry, dt, numSteps);
    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };

    // Act & Assert
    for (const auto &[X, S, I]: simulate(state, simParams, lightModel, stop)) {
        ASSERT_GE(S, 0.0) << "Substrate should never be negative";
    }
}

TEST(SimulateMultipleSteps, CantForceNegativeBiomassWithHighKd) {
    constexpr double dt = 0.01;
    constexpr double numSteps = 1000;
    bool stop = false;
    const MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 5.0};
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry, dt, numSteps);
    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };

    // Act & Assert
    for (const auto &[X, S, I]: simulate(state, simParams, lightModel, stop)) {
        ASSERT_GE(X, 0.0) << "Biomass should never be negative";
    }
}

TEST(SimulateMultipleSteps, BiomassRemainsConstantAfterSubstrateIsZero) {
    // Arrange
    constexpr double dt = 0.1;
    bool stop = false;
    const MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0};
    constexpr int numSteps = 1000; // TODO: the reactor geometry values are hard coded for now.
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry, dt, numSteps);
    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };

    //Act
    const auto results = simulate(state, simParams, lightModel, stop);

    // Assert
    auto it = std::ranges::find_if(results.begin(), results.end(), [](const auto &r) {
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
    const MonodState state{1.0, 10.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0};
    constexpr int numSteps = 10;
    constexpr double dt = 0.01;

    const ReactorGeometry shallow{0.01, 200.0, 0.2}; // 1 cm
    const ReactorGeometry deep{0.20, 200.0, 0.2}; // 20 cm
    const auto simParamsShallow = SimulationParameters(params, shallow, dt, numSteps);
    const auto simParamsDeep = SimulationParameters(params, deep, dt, numSteps);
    const auto lightModelShallow = [&shallow](double X) { return depthAveragedIrradiance(shallow, X); };
    const auto lightModelDeep = [&deep](double X) { return depthAveragedIrradiance(deep, X); };

    bool stop = false;

    const auto shallow_result = simulate(state, simParamsShallow, lightModelShallow, stop);
    const auto deep_result = simulate(state, simParamsDeep, lightModelDeep, stop);

    EXPECT_GT(shallow_result.back().X, deep_result.back().X);
}

TEST(SimulateMultipleSteps, ResultContainsIAvg) {
    // Arrange
    constexpr double dt = 0.01;
    constexpr int numSteps = 1000;
    bool stop = false;

    const MonodState state{50.0, 5.0};
    const MonodParameters params{KS, 1.5, 6.6, 100.0, 0.0};
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto simParams = SimulationParameters(params, geometry, dt, numSteps);
    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };

    // Act
    auto results = simulate(state, simParams, lightModel, stop);

    // Assert
    auto [X, S, Iavg] = results.front();
    EXPECT_NEAR(Iavg, 157.4, 0.1);
}

TEST(SimulateMultipleSteps, StopFlagSetBeforeRunReturnsOnlySeed) {
    // Arrange
    constexpr double dt = 0.01;
    constexpr size_t numSteps = 1000;
    const MonodState state{1.0, 10.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0};
    const ReactorGeometry geometry{0.05, 200.0, 0.2};
    const auto simParams = SimulationParameters(params, geometry, dt, numSteps);
    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };
    bool stop = true;

    // Act
    const auto results = simulate(state, simParams, lightModel, stop);

    // Assert
    EXPECT_EQ(results.size(), 1U) << "stop=true on entry should skip the loop entirely";
}


TEST(SimulateMultipleSteps, StopFlagInterruptsMidRun) {
    // Arrange
    constexpr double dt = 0.01;
    constexpr size_t numSteps = 1000;
    constexpr size_t stopAfter = 5;
    const MonodState state{1.0, 10.0};
    const MonodParameters params{KS, MU_MAX, 0.5, 100.0, 0.0};
    const ReactorGeometry geometry{0.05, 200.0, 0.2};
    const auto simParams = SimulationParameters(params, geometry, dt, numSteps);

    bool stop = false;
    size_t calls = 0;
    const auto countingLightModel = [&](double X) {
        if (++calls > stopAfter) stop = true;
        return depthAveragedIrradiance(geometry, X);
    };

    // Act
    const auto results = simulate(state, simParams, countingLightModel, stop);

    // Assert
    EXPECT_LT(results.size(), numSteps + 1) << "stop should have interrupted before completion";
    EXPECT_GT(results.size(), 1U) << "some iterations should have run before stopping";
}
