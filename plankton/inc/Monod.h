#pragma once
#include "MonodParameters.h"
#include "MonodState.h"

#include <vector>

double Monod(double S, double Ks, double mu_max);

MonodState eulerStep(const MonodState& state, const MonodParameters& params);

std::vector<MonodState> simulate(int num_steps, const MonodState& state, const MonodParameters& params);
