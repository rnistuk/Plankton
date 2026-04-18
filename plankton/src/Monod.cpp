#include "Monod.h"

#include <stdexcept>

#include "BeerLambert.h"
#include "SimulationParameters.h"

// Private functions
void validateState(const MonodState& state) {
    if (state.X < 0) {
        throw std::invalid_argument("Biomass cannot be negative");
    }

    if (state.S < 0) {
        throw std::invalid_argument("Substrate concentration cannot be negative");
    }
}

// Public functions
double Monod(double S, double Ks, double mu_max) {
    return mu_max * S / (Ks + S);
}

double lightLimitedGrowthRate(double S, double I_avg, const MonodParameters& params) {
    return params.mu_max * std::min(
        S / (params.Ks + S),
        I_avg / (params.Ki + I_avg)
    );
}

MonodState eulerStep(const MonodState& state, const MonodParameters& params, double I_avg) {
    MonodState newState = state;
    const double mu = lightLimitedGrowthRate(state.S, I_avg, params);
    const double net_growth = mu - params.kd;
    const double dX = net_growth * state.X * params.dt;
    newState.X += dX;
    const double dS = (mu/params.Yx_s) * state.X * params.dt;
    newState.S -= dS;
    return newState;
}

std::vector<SimulationRecord> simulate(size_t num_steps
                                       , const MonodState &state
                                       , const SimulationParameters& simParams) {
    validateState(state);
    const auto& params = simParams.monod;
    const auto& geometry = simParams.reactor;
    const SimulationRecord currentState(state.X, state.S, depthAveragedIrradiance(geometry, state.X));
    std::vector<SimulationRecord> records;
    records.reserve(num_steps + 1);
    records.push_back(currentState);

    for (size_t i = 0; i < num_steps; ++i) {
        const auto& back = records.back();
        MonodState monodState = eulerStep(MonodState(back.X, back.S), params, back.I_avg);
        records.emplace_back( std::max(monodState.X, 0.0), std::max(monodState.S, 0.0), depthAveragedIrradiance(geometry, monodState.X));
    }
    return records;
}
