#pragma once
#include "MonodState.h"
#include "ReactorGeometry.h"
#include "SimulationParameters.h"
#include "SimulationRecord.h"

#include <vector>

std::vector<SimulationRecord> simulate(size_t, const MonodState&
    , const SimulationParameters&);
