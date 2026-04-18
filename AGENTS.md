# Project goal
Implement a coupled Monod kinetics + Beer-Lambert light attenuation model for algae bioreactor growth in C++, using TDD. Potential portfolio piece targeting Industrial Plankton.

## Coaching preferences
**Pair programming mode - Navigator role.** You are acting as the Navigator in a pair programming session. I am the Driver and will be writing the code. Your role is to observe my implementation and:
- Review for logical errors, security vulnerabilities, or edge cases
- Maintain the 'big picture' of the architecture and project requirements
- Suggest refactors or better patterns without interrupting my flow unless critical
- Ask clarifying questions to ensure we stay aligned with the project goals

## Technical preferences
- C++ (C++23), CLion
- TDD with Google Test
- Test-first development (Red → Green → Refactor cycle)

## The model to implement

Monod kinetics: growth rate as a function of limiting nutrient concentration
Beer-Lambert light attenuation: exponential light decay through dense algae culture
Coupled ODE system: algae density, nutrient concentration, light field
Parameters to expose: max growth rate, half-saturation constant, light extinction coefficient, initial conditions

### Key questions to resolve early

- ODE integration method (Euler to start, Runge-Kutta later)
- Whether to treat the light field as a 1D spatial dimension (PDE) or a depth-averaged simplification (ODE)
- Output format for plotting results

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
- **Data structures**
  - `MonodState` struct: tracks biomass (X) and substrate (S); constructor validates both (throws `std::invalid_argument` for negative values)
  - `MonodParameters` struct: holds biological constants (Ks, µ_max, Yx_s, Ki, kd); constructor validates all fields
  - `SimulationParameters` struct: aggregates `MonodParameters`, `ReactorGeometry`, and `dt` (the integration timestep); constructor validates `dt > 0`
  - `SimulationRecord` struct: output record per step — X, S, and depth-averaged I_avg
- **Euler integration** (`eulerStep`)
  - Signature: `eulerStep(state, params, I_avg, dt)`
  - Single time-step integration using Euler's method
  - Exponential biomass growth with mortality: `dX/dt = (µ - kd) × X`
  - Substrate consumption coupled to biomass production via yield coefficient
  - Stoichiometric mass balance verified (ΔX = ΔS × Y_x/s)
  - Tests cover: zero substrate, positive growth, substrate depletion, mass conservation
- **Multi-step simulation** (`simulate`)
  - Signature: `simulate(num_steps, initial_state, SimulationParameters, LightModel)` returns `vector<SimulationRecord>`
  - `LightModel` is `std::function<double(double X)>` — caller injects the light model (DIP); Beer-Lambert is the default via lambda
  - Returns initial state + num_steps records (e.g., 10 steps = 11 records total)
  - Each record captures X, S, and I_avg at that step
  - Clamps X and S to zero to prevent negative values (numerical safeguard)
  - Tests cover: growth/depletion, substrate never negative, biomass never negative under high kd, biomass constant after substrate depletion, deeper reactor reduces growth, I_avg recorded correctly
- **Parameter and state validation**
  - All `MonodParameters`, `MonodState`, `ReactorGeometry`, and `SimulationParameters` fields validated in constructors — invalid objects are unconstructable
  - `dt` lives in `SimulationParameters`, not `MonodParameters` (numerical concern, not biological)
  - `validateState()` removed — superseded by `MonodState` constructor
  - Tests verify exceptions for all invalid inputs
- **ReactorGeometry structure**
  - Defines physical reactor parameters: depth (m), I0 (surface irradiance), k (extinction coefficient)
  - Constructor validates all parameters; throws `std::invalid_argument` for invalid values
- **Beer-Lambert light attenuation** (`beerLambert`, `depthAveragedIrradiance`)
  - `beerLambert(z, geometry, X)`: point irradiance I(z) = I₀ × exp(-k × X × z)
  - `depthAveragedIrradiance(geometry, X)`: depth-integrated average over full reactor depth
  - Near-zero biomass guard: checks `k × X × depth < 1e-15` (returns I₀); prevents NaN from denormal X values
- **Light-limited growth** (`lightLimitedGrowthRate`)
  - Implements Liebig's Law: µ = µ_max × min(S/(Ks+S), I_avg/(Ki+I_avg))
  - `eulerStep` requires explicit `I_avg` argument — callers always provide a value
- **Mortality/decay term** — `kd` in `MonodParameters`; enables growth → peak → decline dynamics
- **CSV export** (`writeCsv`) — header + fixed-precision time series to any `ostream`
- **Demo program** (`cli/main.cpp`) — 1000-step simulation with realistic phytoplankton parameters, CSV to stdout
- **Module separation** — `Simulation.cpp` owns `simulate()`; `Monod.cpp` owns kinetics and integration only
- **Public/internal header split**
  - Public: `Simulation.h`, `SimulationParameters.h`, `MonodParameters.h`, `MonodState.h`, `ReactorGeometry.h`, `SimulationRecord.h`, `CsvExport.h`
  - Internal: `Monod.h`, `BeerLambert.h`
- **Static library** (`plankton_lib`) — simulation sources as STATIC library; `PlanktonTests` links against it; `cli/` links against it
- **CMake** — `PLANKTON_SOURCES` lists `.cpp` files only (no headers); `PlanktonTests` links `plankton_lib` rather than recompiling sources; Google Test integration


### ❌ Not started
- **Separate N and P tracking**: Break out nitrogen and phosphorus as separate state variables instead of generic substrate S
  - Update `MonodState` to include N and P fields
  - Update `MonodParameters` to include separate Ks_N, Ks_P, Yx_N, Yx_P
  - Implement dual nutrient limitation (Liebig's law of the minimum)
  - Update main.cpp output to show t, X, N, P
  - Update tests for new structure
- **Advanced integration methods**: Runge-Kutta 2nd or 4th order
- **Generic integration refactoring**: Extract `eulerStep()` into model-agnostic numerical library
  - Create generic `eulerStep(state, dt, derivative_function)` that works with any ODE system
  - Make integration methods reusable across projects
  - Move model-specific logic (Monod kinetics, stoichiometry) into derivative function
  - Enable easier addition of RK2/RK4 without duplicating model logic
- **1D spatial light profile**: Extend from depth-averaged to PDE

## Design decisions made

1. **ODE integration method**: Euler's method implemented first; Runge-Kutta planned for later
2. **Light representation**: Will start with depth-averaged scalar, extend to 1D profile later
3. **Output format**: CSV planned for plotting compatibility (Python, Excel, gnuplot)
4. **Testing strategy**: Pure functions first (Monod), then integration (eulerStep), then simulation loop
5. **Data structures**: Separate `State` (variables) from `Parameters` (constants)
6. **Mass balance**: Explicit yield coefficient (Y_x/s) to couple biomass growth and substrate consumption
7. **Programming paradigm**: Functional style with pure functions and immutable data; domain constraints enforced at construction (all structs); low-level functions (`eulerStep`) trust their inputs
8. **Integration timestep placement**: `dt` belongs in `SimulationParameters`, not `MonodParameters` — it is a numerical concern, not a biological constant
9. **Light model injection**: `simulate()` accepts a `LightModel` (`std::function<double(double)>`) rather than calling `depthAveragedIrradiance()` directly — Beer-Lambert injected via lambda at call sites (DIP)

## Key assumptions
- Depth-averaged light intensity (for initial version)
- No spatial gradients (well-mixed reactor assumption)
- Temperature and pH are optimal (not explicitly modeled)

## Next steps

### Future enhancements
- **Template `simulate()`**: Replace `std::function<double(double)>` `LightModel` with a template parameter to eliminate `std::function` overhead while preserving the injectable light model design
- **Runge-Kutta integration** (RK2 or RK4) for improved accuracy
- **Generic integration refactoring**: Extract `eulerStep()` into a model-agnostic integration function — `integrate(state, dt, derivative_fn)` — enabling RK2/RK4 without duplicating model logic
- **Adaptive time stepping**
- **Separate N and P tracking**: Replace generic substrate `S` with distinct nitrogen and phosphorus state variables; dual nutrient limitation via Liebig's Law
  - `MonodState`: add N and P fields
  - `MonodParameters`: add Ks_N, Ks_P, Yx_N, Yx_P
  - Update `cli/` output to show t, X, N, P
- **1D spatial light profile**: Extend from depth-averaged scalar to PDE
- **Performance benchmarking and parallelization** for parameter sweeps