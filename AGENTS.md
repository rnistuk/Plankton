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
- **Monod growth rate function** (`Monod(S, Ks, mu_max)`)
  - Returns specific growth rate µ as a function of substrate concentration
  - Comprehensive unit tests covering edge cases (zero substrate, half-saturation, high substrate, monotonicity)
- **Data structures**
  - `MonodState` struct: tracks biomass (X) and substrate (S) concentrations — used as integration state
  - `MonodParameters` struct: holds model constants (K_s, µ_max, Y_x/s, Ki, kd, dt); constructor validates all parameters at construction (throws `std::invalid_argument`)
  - `SimulationRecord` struct: output record per step — X, S, and depth-averaged I_avg
  - `SimulationParameters` struct: aggregates `MonodParameters` and `ReactorGeometry` into a single configuration type (defined, not yet used in simulation API)
- **Euler integration** (`eulerStep`)
  - Single time-step integration using Euler's method
  - Exponential biomass growth (dX/dt = µ × X)
  - Substrate consumption coupled to biomass production via yield coefficient
  - Stoichiometric mass balance verified (ΔX = ΔS × Y_x/s)
  - Tests cover: zero substrate, positive growth, substrate depletion, mass conservation
- **Multi-step simulation** (`simulate`)
  - Runs multiple integration steps and returns time series
  - Signature: `simulate(num_steps, initial_state, SimulationParameters)` returns `vector<SimulationRecord>`
  - `SimulationParameters` aggregates `MonodParameters` and `ReactorGeometry` — the public API entry point
  - Returns initial state + num_steps (e.g., 10 steps = 11 records total)
  - Each record captures X, S, and depth-averaged I_avg at that step
  - Test verifies biomass increases and substrate decreases over time
  - Clamps substrate `S` to zero to prevent negative values (numerical safeguard)
  - Clamps biomass `X` to zero to prevent negative values under high mortality
  - Test verifies substrate never goes negative throughout simulation
  - Test verifies biomass never goes negative under high `kd`
  - Test verifies biomass remains constant after substrate depletion
- **Parameter validation**
  - All `MonodParameters` fields validated in the constructor; invalid objects are unconstructable
  - Throws `std::invalid_argument` with descriptive messages for invalid inputs
  - `validateParameters()` external function removed — constructor enforcement is sufficient
  - Tests verify proper exceptions for: negative/zero Ks, mu_max, Yx_s, Ki, dt; negative kd
- **State validation** (`validateState`)
  - Validates initial MonodState before simulation starts
  - Throws `std::invalid_argument` for negative biomass or substrate
  - Tests verify proper exceptions for: negative X, negative S
- **ReactorGeometry structure**
  - Defines physical reactor parameters: depth (m), I0 (surface irradiance), k (extinction coefficient)
  - Constructor validates all parameters (depth > 0, I0 > 0, k > 0)
  - Throws `std::invalid_argument` for invalid parameters
  - Tests verify construction and validation behavior
- **Beer-Lambert light attenuation** (`beerLambert`, `depthAveragedIrradiance`)
  - `beerLambert(z, geometry, X)`: point irradiance I(z) = I₀ × exp(-k × X × z)
  - `depthAveragedIrradiance(geometry, X)`: depth-integrated average over full reactor depth
  - Handles near-zero biomass edge case: guard checks `k × X × depth < 1e-15` (returns I₀); prevents division-by-zero and NaN from denormal X values
  - Tests cover: surface boundary condition, monotonicity with depth and biomass, analytical value, zero biomass
- **Light-limited growth** (`lightLimitedGrowthRate`)
  - Implements Liebig's Law: µ = µ_max × min(S/(Ks+S), I_avg/(Ki+I_avg))
  - `Ki` (light half-saturation constant) added to `MonodParameters` with constructor validation
  - `eulerStep` requires explicit `I_avg` argument (no default); callers must always provide a value
  - Tests cover: zero light stops growth, light at half-saturation, substrate-limiting case
- **Mortality/decay term**
  - Added specific death rate `kd` to `MonodParameters` and `eulerStep`
  - `dX/dt = µ × X - kd × X`
  - Constructor validation ensures `kd >= 0`
  - Simulation now correctly captures growth, peak, and decline dynamics
  - Tests verify mortality behavior with and without substrate
- **Configurable reactor geometry in `simulate()`**
  - `ReactorGeometry` is now passed into `simulate()` — no hardcoded values
  - Test verifies deeper reactor produces less growth than shallow reactor
- **CSV export** (`writeCsv`)
  - Writes header and fixed-precision time series to any `ostream`
  - Format: t (2dp), X, S, I_avg (4dp each), comma-space separated
  - `writeRecord` helper in anonymous namespace
- **Demo program** (`cli/main.cpp`)
  - Runs 1000-step simulation with realistic phytoplankton parameters
  - Outputs CSV to stdout via `writeCsv`
  - Includes documented parameter ranges for algae cultures
- **Public/internal header split**
  - Public headers (safe to include by consumers): `Simulation.h`, `SimulationParameters.h`, `MonodParameters.h`, `MonodState.h`, `ReactorGeometry.h`, `SimulationRecord.h`, `CsvExport.h`
  - Internal headers (implementation details, not exposed): `Monod.h`, `BeerLambert.h`
- **`cli/` subdirectory** — separate CMake project for the command-line interface; links against the simulation library
- **CMake build system** with Google Test integration

### 🚧 In progress
- **Static library refactor**: Extract simulation code into `plankton_lib` (STATIC); `cli/` and future GUI link against it. Use CMake `PLANKTON_SOURCES` variable to DRY up source lists. `PlanktonTests` compiles sources directly (not via library) to test individual modules.

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
7. **Programming paradigm**: Functional style with pure functions and immutable data; domain constraints enforced at construction (structs) and at simulation entry (`validateState`); low-level functions (`eulerStep`) trust their inputs

## Key assumptions
- Depth-averaged light intensity (for initial version)
- No spatial gradients (well-mixed reactor assumption)
- Temperature and pH are optimal (not explicitly modeled)

## Next steps

### Immediate priorities
1. **Complete static library refactor**: Define `PLANKTON_SOURCES` variable, add `plankton_lib` target, update `cli/CMakeLists.txt` to link against it, and flesh out `cli/main.cpp` with the full simulation and CSV output
2. **Extended simulation test**: Run longer simulations (1000+ steps) to verify numerical stability with mortality, light coupling and self-shading

### Future enhancements
- Runge-Kutta integration (RK2 or RK4) for improved accuracy
- Adaptive time stepping
- Separate N and P tracking with dual nutrient limitation
- 1D spatial light profile (depth-dependent growth)
- Performance benchmarking
- Parallelization for parameter sweeps