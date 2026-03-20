#include "Monod.h"

double Monod(double S, double Ks, double mu_max) {
    return mu_max * S / (Ks + S);
}

MonodState eulerStep(const MonodState& state, const MonodParameters& params) {
    MonodState newState = state;
    double mu = Monod(state.S, params.Ks, params.mu_max);
    double dX = mu * state.X * params.dt;
    newState.X += dX;
    double dS = dX / params.Yx_s;
    newState.S -= dS;
    return newState;
}

std::vector<MonodState> simulate(int num_steps, const MonodState& state, const MonodParameters& params) {
    std::vector<MonodState> result;
    result.reserve(num_steps + 1);
    result.push_back(state);
    for (int i = 0; i < num_steps; ++i) {
        auto next = eulerStep(result.back(), params);
        next.S = std::max(next.S, 0.0); // The substrate concentration cannot be negative in a physical system, so we fake that here.
        result.push_back(next);
    }
    return result;
}
