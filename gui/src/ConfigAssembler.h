#pragma once

#include "MonodState.h"
#include "SimulationParameters.h"

class QWizard;

namespace plankton::gui {
    struct RunConfig {
        MonodState initialState;
        SimulationParameters params;
        int numSteps;
    };

    class ConfigAssembler {
    public:
        RunConfig assemble(const QWizard* wizard);
    };
}
