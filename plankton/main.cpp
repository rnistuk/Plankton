#include "CsvExport.h"
#include "Monod.h"
#include "Simulation.h"
#include <iostream>

/*
Initial State
    X (biomass): 10-100 μg C/mL for a typical culture, up to 1000+ μg C/mL for dense bioreactor cultures
    S (nutrient):
    Nitrogen: 1-10 mg N/L (nitrate) for batch culture
    Phosphorus: 0.1-1 mg P/L (phosphate)
Monod Parameters
    μ_max (max growth rate):
        0.5-2.0 day⁻¹ for most phytoplankton
        Fast growers (like Nannochloropsis): 1-2 day⁻¹
        Slow growers (like diatoms): 0.5-1 day⁻¹
    Ks (half-saturation constant):
        Nitrogen: 0.1-5 mg N/L (typical: 1-2)
        Phosphorus: 0.01-0.5 mg P/L (typical: 0.05-0.1)
    Ki (half-saturation constant for light)
        200 µmol photons m⁻² s⁻¹, with 50–100 being typical for most green
        microalgae.
        Fast-growing species like Chlorella tend toward the lower end (~20–50);
        slower or shade-adapted species can be higher.
    Yx/s (yield coefficient):
        For nitrogen: ~5-10 mg C / mg N (from Redfield 106:16 ≈ 6.6)
        For phosphorus: ~50-100 mg C / mg P (from Redfield 106:1 = 106)
    kd (specific death rate):
        0.05-0.3 day⁻¹ for phytoplankton
        0.0 = pure growth model (no mortality)
    dt: 0.01-0.1 days (0.25-2.4 hours) for stability
 */
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
