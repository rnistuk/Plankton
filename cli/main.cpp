#include "Simulation.h"
#include "SimulationParameters.h"
#include "CsvExport.h"

#include <iostream>

int main() {
    // TODO: the reactor geometry values are hard coded for now.
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    constexpr double Kd = 0.3; //
    const auto geometry = ReactorGeometry(depth, I0, k);
    constexpr MonodState initial_state{0.05,5.0};
    const MonodParameters params{1.0, 1.5, 6.6, 100.0, Kd, 0.01};
    const SimulationParameters simParams(params, geometry);
    constexpr int num_steps = 1000;
    const auto results = simulate(num_steps
        , initial_state, simParams);
    std::cout << "\nSimulated Monod plankton growth\n";
    writeCsv(std::cout, results, params.dt);
    return 0;
}