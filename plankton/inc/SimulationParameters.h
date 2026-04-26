#pragma once
#include "MonodParameters.h"
#include "ReactorGeometry.h"

struct SimulationParameters {
    MonodParameters monod;
    ReactorGeometry reactor;     // constants
    double dt;
    size_t numSteps;

    SimulationParameters(const MonodParameters& monod
        , const ReactorGeometry& reactor, double dt, size_t numSteps)
        : monod(monod), reactor(reactor), dt(dt), numSteps(numSteps) {
        if (this->dt <= 0) {
            throw std::invalid_argument("The timestep, dt, cannot be negative or zero");
        }
    }
};
