#include "SimulationRunner.h"

SimulationRunner::SimulationRunner(const SimulationParameters& sp, const LightModel& lm) : monodState(1,1),
    simulationParameters(sp), lightModel(lm), running(false) {
}

void SimulationRunner::start(const MonodState &ms) {

}

void SimulationRunner::stop() {

}


