#include "Monod.h"
#include <iomanip>
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
    Yx/s (yield coefficient):
        For nitrogen: ~5-10 mg C / mg N (from Redfield 106:16 ≈ 6.6)
        For phosphorus: ~50-100 mg C / mg P (from Redfield 106:1 = 106)
    dt: 0.01-0.1 days (0.25-2.4 hours) for stability
 */
int main() {

    MonodState initial_state{50.0,5.0};
    MonodParameters params{1.0, 1.5, 6.6, 0.01};
    int num_steps = 100;

    auto result = simulate(num_steps, initial_state, params);

    double t{0};
    std::cout << "\nSimulated Monod plankton growth" << std::endl;
    std::cout << "t(days), X(biomass,μg C/mL), S(nutrient, mg N/L or mg P/L)" << std::endl;
    for (auto& state : result) {
        std::cout << std::fixed << std::setprecision(2) << t << ", "
        << std::setprecision(4) << state.X << ", " << state.S << std::endl;
        t += params.dt;
    }
    std::cout << std::endl;

    return 0;
}