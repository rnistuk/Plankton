#pragma once
#include "Simulation.h"

class SimulationRunner {
public:
    SimulationRunner(const SimulationParameters& , const LightModel&);
    MonodState& getMonodState() { return this->monodState; }
    SimulationParameters& getSimulationParameters() { return this->simulationParameters; }
    void start(const MonodState&);
    void stop();

    std::vector<SimulationRecord>& getResults();

private:
    MonodState monodState;
    SimulationParameters simulationParameters;
    LightModel lightModel;
    bool running;
};
