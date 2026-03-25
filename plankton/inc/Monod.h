#pragma once
#include "MonodParameters.h"
#include "MonodState.h"
#include "ReactorGeometry.h"

#include <limits>
#include <vector>

double Monod(double S, double Ks, double mu_max); // To be replaced by lightLimitedGrowthRate

double lightLimitedGrowthRate(double S, double I_avg, const MonodParameters& params);

MonodState eulerStep(const MonodState& state, const MonodParameters& params, double I_avg = std::numeric_limits<double>::infinity());

std::vector<MonodState> simulate(int num_steps, const MonodState& state
    , const MonodParameters& params
    , const ReactorGeometry& geometry);
