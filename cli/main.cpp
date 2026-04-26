#include "Simulation.h"
#include "CsvExport.h"

#include <iostream>

#include "BeerLambert.h"

int main() {
    // TODO: the reactor geometry values are hard coded for now.
    constexpr double depth = 0.05; // 5 cm
    constexpr double I0 = 200.0; // moderate sunlight
    constexpr double k = 0.2;
    constexpr double Kd = 0.3; //
    constexpr double dt = 0.01;
    constexpr int num_steps = 1000;
    const auto geometry = ReactorGeometry(depth, I0, k);


    const MonodParameters params{1.0, 1.5, 6.6, 100.0, Kd};


    const MonodState initial_state(0.05,5.0);
    const SimulationParameters simParams(params, geometry, dt, num_steps);
    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };
    bool stop = false;

    const auto results = simulate( initial_state, simParams
        , lightModel, stop);
    std::cout << "\nSimulated Monod plankton growth\n";
    writeCsv(std::cout, results, simParams.dt);
    return 0;
}
