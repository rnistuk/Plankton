#include "ConfigAssembler.h"
#include <QWizard>
#include <QVariant>
namespace plankton::gui {
    RunConfig ConfigAssembler::assemble(const QWizard* wizard) {
        // 1. Extract raw data from Wizard fields
        double X = wizard->field("initialConditions.X").toDouble();
        double S = wizard->field("initialConditions.S").toDouble();
        double dt = wizard->field("simulation.dt").toDouble();
        int numSteps = wizard->field("simulation.numSteps").toInt();

        double Ks     = wizard->field("kinetics.Ks").toDouble();
        double mu_max = wizard->field("kinetics.mu_max").toDouble();
        double Yx_s   = wizard->field("kinetics.Yx_s").toDouble();
        double Ki     = wizard->field("kinetics.Ki").toDouble();
        double kd     = wizard->field("kinetics.kd").toDouble();

        double depth = wizard->field("reactor.depth").toDouble();
        double I0    = wizard->field("reactor.I0").toDouble();
        double k     = wizard->field("reactor.k").toDouble();

        // 2. Construct domain objects
        // Note: These constructors may throw std::invalid_argument if inputs are invalid.
        const auto geometry = ReactorGeometry(depth, I0, k);
        const auto initial_state = MonodState(X, S);
        const MonodParameters kinetics(Ks, mu_max, Yx_s, Ki, kd);
        const SimulationParameters simParams(kinetics, geometry, dt);

        return { initial_state, simParams, numSteps };
    }
}
