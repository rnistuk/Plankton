#include "Monod.h"

double Monod(double S, double Ks, double mu_max) {
    return mu_max * S / (Ks + S);
}

MonodState eulerStep(const MonodState& state, const MonodParameters& params) {
    MonodState newState = state;
    double mu = Monod(state.S, params.Ks, params.mu_max);
    double dX = mu * state.X * params.dt;
    newState.X += dX;
    double dS = dX/ params.Yx_s;
    newState.S -= dS;
    return newState;
}
