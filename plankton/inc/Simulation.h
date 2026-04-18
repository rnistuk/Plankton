#pragma once
#include "MonodState.h"
#include "ReactorGeometry.h"
#include "SimulationParameters.h"
#include "SimulationRecord.h"

#include <functional>
#include <vector>

using LightModel = std::function<double(double X)>;
std::vector<SimulationRecord> simulate(size_t, const MonodState&
    , const SimulationParameters&, const LightModel&);
