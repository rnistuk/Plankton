#pragma once
#include "MonodState.h"
#include "SimulationParameters.h"
#include "SimulationRecord.h"

#include <functional>
#include <vector>

using LightModel = std::function<double(double X)>;

std::vector<SimulationRecord> simulate(const MonodState&
    , const SimulationParameters&, const LightModel&, bool& stop);
