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
    double mu = lightLimitedGrowthRate(state.S, I_avg, params);
    double dX = mu * state.X * params.dt;
    newState.X += dX;
    double dS = dX / params.Yx_s;
    newState.S -= dS;
    return newState;
}

std::vector<MonodState> simulate(int num_steps, const MonodState& state, const MonodParameters& params) {
    validateParameters(params);
    validateState(state);

    // TODO: the reactor geometry values are hard coded for now.
    double depth = 0.05; // 5 cm
    double I0 = 200.0; // moderate sunlight
    double k = 0.2;

    auto geometry = ReactorGeometry(depth, I0, k);
    std::vector<MonodState> result;
    result.reserve(num_steps + 1);
    result.push_back(state);
    for (int i = 0; i < num_steps; ++i) {
        const auto I_avg = depthAveragedIrradiance(geometry, result.back().X);
        auto next = eulerStep(result.back(), params, I_avg);
        next.S = std::max(next.S, 0.0); // The substrate concentration cannot be negative in a physical system, so we fake that here.
        result.push_back(next);
    }
    return result;
}
