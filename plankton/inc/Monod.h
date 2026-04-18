#pragma once
#include "MonodParameters.h"
#include "MonodState.h"

double lightLimitedGrowthRate(double S, double I_avg, const MonodParameters& params);

MonodState eulerStep(const MonodState& state, const MonodParameters& params
                     , double I_avg, double dt);
