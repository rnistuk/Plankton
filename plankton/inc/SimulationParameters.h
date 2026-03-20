#pragma once
#include "MonodParameters.h"
#include "ReactorGeometry.h"

struct SimulationParameters {
    MonodParameters monod;
    ReactorGeometry reactor;     // constants
};
