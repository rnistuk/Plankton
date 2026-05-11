#include "Monod.h"

#include <algorithm>

double lightLimitedGrowthRate(const double S, const double I_avg, const MonodParameters &params) {
    return params.mu_max * std::min(
               S / (params.Ks + S),
               I_avg / (params.Ki + I_avg)
           );
}

MonodState eulerStep(const MonodState &state, const MonodParameters &params
                     , const double I_avg, const double dt) {
    MonodState newState = state;
    const double mu = lightLimitedGrowthRate(state.S, I_avg, params);
    const double net_growth = mu - params.kd;
    const double dX = net_growth * state.X * dt;
    newState.X += dX;
    const double dS = (mu / params.Yx_s) * state.X * dt;
    newState.S -= dS;
    return newState;
}
