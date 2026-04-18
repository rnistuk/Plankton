#include "BeerLambert.h"
#include "Monod.h"
#include "Simulation.h"

std::vector<SimulationRecord> simulate(size_t num_steps
                                       , const MonodState &state
                                       , const SimulationParameters& simParams
                                       , const LightModel& lightModel) {
    const auto& params = simParams.monod;
    const SimulationRecord currentState(state.X, state.S, lightModel(state.X));
    std::vector<SimulationRecord> records;
    records.reserve(num_steps + 1);
    records.push_back(currentState);

    for (size_t i = 0; i < num_steps; ++i) {
        const auto& back = records.back();
        auto [X, S] = eulerStep(MonodState(back.X, back.S), params
                                , back.I_avg, simParams.dt);
        X = std::max(X, 0.0);
        S = std::max(S, 0.0);
        records.emplace_back( X, S , lightModel(X));
    }
    return records;
}
