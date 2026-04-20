#include "BeerLambert.h"
#include "CsvExport.h"
#include "Simulation.h"

#include <iostream>

int main() {
    constexpr double X = 0.05;      // Biomass concentration (0, ~10] g/L
    constexpr double S = 5.0;       // Substrate concentration [0, ~5] g/L

    constexpr double depth = 0.05;  // Reactor Depth (0, ~0.3) metres
    constexpr double I0 = 200.0;    // Surface Irradiance (0, ~2000] µmol photons m⁻² s⁻¹
    constexpr double mu_max = 1.5;  // Max specific growth rate (0, ~3] day⁻¹
    constexpr double Yx_s = 6.6;    // Biomass yield coefficient (0, ~10] g biomass / g substrate

    constexpr double k = 0.2;       // Extinction coefficient (0, ~2] L g⁻¹ m⁻¹
    constexpr double Kd = 0.3;      // Mortality / decay rate [0, ~0.5] L g⁻¹ m⁻¹

    constexpr double Ki = 100.0;    // Light half-saturation constant (0, ~500] µmol photons m⁻² s⁻¹
    constexpr double Ks = 1.0;      // Substrate half-saturation constant (0, ~1] g/L

    constexpr double dt = 0.01;     // Time step (0, ~0.1] days
    constexpr int num_steps = 1000;

    const auto geometry = ReactorGeometry(depth, I0, k);
    const auto initial_state = MonodState(X, S);

    const auto lightModel = [&geometry](double X) { return depthAveragedIrradiance(geometry, X); };

    const MonodParameters params{Ks, mu_max, Yx_s, Ki, Kd};
    const SimulationParameters simParams(params, geometry, dt);

    const auto results = simulate(num_steps
        , initial_state, simParams, lightModel);

    std::cout << "\nSimulated Monod plankton growth\n";
    writeCsv(std::cout, results, simParams.dt);
    return 0;
}
