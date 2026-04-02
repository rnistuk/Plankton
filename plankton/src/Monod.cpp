#include "Monod.h"

#include <stdexcept>

#include "BeerLambert.h"

// Private functions
void validateParameters(const MonodParameters& params) {
    if (params.Ks <= 0) {
        throw std::invalid_argument("Ks must be positive");
    }

    if (params.mu_max <= 0) {
        throw std::invalid_argument("mu_max must be positive");
    }

    if (params.Yx_s <= 0) {
        throw std::invalid_argument("Yx_s must be positive");
    }

    if (params.dt <= 0) {
        throw std::invalid_argument("dt must be positive");
    }
}

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
    const double dX = mu * state.X * params.dt;
    newState.X += dX;
    const double dS = dX / params.Yx_s;
    newState.S -= dS;
    return newState;
}

std::vector<SimulationRecord> simulate(int num_steps
                                       , const MonodState &state
                                       , const MonodParameters &params
                                       , const ReactorGeometry &geometry) {
    validateParameters(params);
    validateState(state);
    SimulationRecord currentState(state.X, state.S, depthAveragedIrradiance(geometry, state.X));
    std::vector<SimulationRecord> records;
    records.reserve(num_steps + 1);
    records.push_back(currentState);

    for (int i = 0; i < num_steps; ++i) {
        auto& back = records.back();
        MonodState monodState = eulerStep(MonodState(back.X, back.S), params, back.I_avg);
        records.push_back(SimulationRecord(monodState.X, std::max(monodState.S, 0.0), depthAveragedIrradiance(geometry, monodState.X)));
    }
    return records;
}
