# Project goal
Implement a coupled Monod kinetics + Beer-Lambert light attenuation model for algae bioreactor growth in C++, using TDD. Potential portfolio piece targeting Industrial Plankton.

## Coaching preferences
**Guide step-by-step.** Provide guidance and design questions rather than complete code solutions unless explicitly requested.

## Technical preferences
- C++ (C++23), CLion
- TDD with Google Test
- Test-first development (Red → Green → Refactor cycle)

## The model to implement

Monod kinetics: growth rate as a function of limiting nutrient concentration
Beer-Lambert light attenuation: exponential light decay through dense algae culture
Coupled ODE system: algae density, nutrient concentration, light field
Parameters to expose: max growth rate, half-saturation constant, light extinction coefficient, initial conditions

Key questions to resolve early

ODE integration method (Euler to start, Runge-Kutta later)
Whether to treat the light field as a 1D spatial dimension (PDE) or a depth-averaged simplification (ODE)
Output format for plotting results

## Key considerations
- Ensure numerical stability and accuracy in ODE integration
- Validate input parameters early (fail fast with clear errors)
- Keep domain logic (kinetics) separate from orchestration (simulation loop)
- Use tolerance-based assertions for floating-point tests
- Export results in plotting-friendly formats (CSV first)
- Document assumptions (e.g., depth-averaged light, no mortality term)
- Consider biomass yield coefficient for nutrient consumption

## Current state

### ✅ Completed
- **Monod growth rate function** (`Monod(S, Ks, mu_max)`)
  - Returns specific growth rate µ as a function of substrate concentration
  - Comprehensive unit tests covering edge cases (zero substrate, half-saturation, high substrate, monotonicity)
- **Data structures**
  - `MonodState` struct: tracks biomass (X) and substrate (S) concentrations
  - `MonodParameters` struct: holds model constants (K_s, µ_max, Y_x/s, dt)
- **Euler integration** (`eulerStep`)
  - Single time-step integration using Euler's method
  - Exponential biomass growth (dX/dt = µ × X)
  - Substrate consumption coupled to biomass production via yield coefficient
  - Stoichiometric mass balance verified (ΔX = ΔS × Y_x/s)
  - Tests cover: zero substrate, positive growth, substrate depletion, mass conservation
- **CMake build system** with Google Test integration

### 🚧 In progress
- None currently

### ❌ Not started
- **Multi-step simulation testing**: Verify behavior over many time steps
- **Simulation loop function**: `simulate(initial_state, params, num_steps)` to run multiple steps
- **Parameter validation**: Check for invalid inputs (negative values, dt ≤ 0, etc.)
- **Substrate depletion handling**: Prevent substrate from going negative
- **CSV export**: Write simulation results for plotting
- **Beer-Lambert light attenuation**: I(z) = I₀ × exp(-k × X × z)
- **Light-limited growth**: Couple light intensity to Monod growth rate
- **Advanced integration methods**: Runge-Kutta 2nd or 4th order
- **1D spatial light profile**: Extend from depth-averaged to PDE

## Design decisions made

1. **ODE integration method**: Euler's method implemented first; Runge-Kutta planned for later
2. **Light representation**: Will start with depth-averaged scalar, extend to 1D profile later
3. **Output format**: CSV planned for plotting compatibility (Python, Excel, gnuplot)
4. **Testing strategy**: Pure functions first (Monod), then integration (eulerStep), then simulation loop
5. **Data structures**: Separate `State` (variables) from `Parameters` (constants)
6. **Mass balance**: Explicit yield coefficient (Y_x/s) to couple biomass growth and substrate consumption

## Key assumptions
- No mortality or maintenance respiration terms (pure growth model)
- Depth-averaged light intensity (for initial version)
- No spatial gradients (well-mixed reactor assumption)
- Temperature and pH are optimal (not explicitly modeled)

## Next steps

### Immediate priorities
1. **Multi-step simulation test**: Write test that runs 10-100 time steps, verify biomass increases and substrate decreases over time
2. **Simulation loop**: Implement `simulate()` function that returns time series of states
3. **Substrate depletion guard**: Ensure substrate cannot go negative (biological constraint)

### Near-term goals
4. **Parameter validation**: Fail fast with clear errors for invalid inputs
5. **CSV export**: Write results in column format (time, X, S) for plotting
6. **Beer-Lambert function**: Implement light attenuation model
7. **Couple light to growth**: Modify growth rate based on available light

### Future enhancements
- Runge-Kutta integration (RK2 or RK4) for improved accuracy
- Adaptive time stepping
- 1D spatial light profile (depth-dependent growth)
- Performance benchmarking
- Parallelization for parameter sweeps