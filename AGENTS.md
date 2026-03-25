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
  - `MonodState` struct: tracks biomass (X) and substrate (S) concentrations
  - `MonodParameters` struct: holds model constants (K_s, µ_max, Y_x/s, Ki, dt)
- **Euler integration** (`eulerStep`)
  - Single time-step integration using Euler's method
  - Exponential biomass growth (dX/dt = µ × X)
  - Substrate consumption coupled to biomass production via yield coefficient
  - Stoichiometric mass balance verified (ΔX = ΔS × Y_x/s)
  - Tests cover: zero substrate, positive growth, substrate depletion, mass conservation
- **Multi-step simulation** (`simulate`)
  - Runs multiple integration steps and returns time series
  - Signature: `simulate(num_steps, initial_state, params, geometry)` returns `vector<MonodState>`
  - Returns initial state + num_steps (e.g., 10 steps = 11 states total)
  - Test verifies biomass increases and substrate decreases over time
  - Clamps substrate to zero to prevent negative values (numerical safeguard)
  - Test verifies substrate never goes negative throughout simulation
  - Test verifies biomass remains constant after substrate depletion
- **Parameter validation** (`validateParameters`)
  - Validates all MonodParameters before simulation starts
  - Throws `std::invalid_argument` with descriptive messages for invalid inputs
  - Tests verify proper exceptions for: negative/zero Ks, mu_max, Yx_s, dt
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
  - Handles X = 0 edge case (returns I₀ — no biomass, no attenuation)
  - Tests cover: surface boundary condition, monotonicity with depth and biomass, analytical value, zero biomass
- **Light-limited growth** (`lightLimitedGrowthRate`)
  - Implements Liebig's Law: µ = µ_max × min(S/(Ks+S), I_avg/(Ki+I_avg))
  - `Ki` (light half-saturation constant) added to `MonodParameters` with constructor validation
  - `eulerStep` updated to accept `I_avg`; defaults to `∞` for backward compatibility
  - Tests cover: zero light stops growth, light at half-saturation, substrate-limiting case
- **Configurable reactor geometry in `simulate()`**
  - `ReactorGeometry` is now passed into `simulate()` — no hardcoded values
  - Test verifies deeper reactor produces less growth than shallow reactor
- **Demo program** (`main.cpp`)
  - Runs 100-step simulation with realistic phytoplankton parameters
  - Outputs time series data: t, X (biomass), S (nutrient)
  - Includes documented parameter ranges for algae cultures
- **CMake build system** with Google Test integration

### ❌ Not started
- **Mortality/decay term**: Add specific death rate `kd` to `MonodParameters` and `eulerStep`
  - `dX/dt = µ × X - kd × X`
  - Typical values: 0.05–0.3 day⁻¹ for phytoplankton
  - Produces growth → peak → decline dynamics; makes self-shading effects visible in plots
  - Add `kd` validation (must be non-negative; zero = current pure-growth behaviour)
- **CSV export**: Write simulation results for plotting
- **Separate N and P tracking**: Break out nitrogen and phosphorus as separate state variables instead of generic substrate S
  - Update `MonodState` to include N and P fields
  - Update `MonodParameters` to include separate Ks_N, Ks_P, Yx_N, Yx_P
  - Implement dual nutrient limitation (Liebig's law of the minimum)
  - Update main.cpp output to show t, X, N, P
  - Update tests for new structure
- **Advanced integration methods**: Runge-Kutta 2nd or 4th order
- **Refactor validation to use constructor pattern**: Move validation from external functions to constructors
  - Add constructors to MonodParameters and MonodState with validation
  - Remove external validateParameters() and validateState() functions
  - Consistent with ReactorGeometry's constructor validation approach
  - Makes it impossible to create invalid objects (fail-fast at construction)
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
7. **Programming paradigm**: Functional style with pure functions and immutable data; domain constraints handled at orchestration level (`simulate()`), not in low-level functions (`eulerStep()`)

## Key assumptions
- No mortality or maintenance respiration terms (pure growth model) — **planned for removal**
- Depth-averaged light intensity (for initial version)
- No spatial gradients (well-mixed reactor assumption)
- Temperature and pH are optimal (not explicitly modeled)

## Next steps

### Immediate priorities
1. **Mortality/decay term**: Add `kd` to `MonodParameters` and `eulerStep` to enable growth → peak → decline dynamics
2. **CSV export**: Write simulation results in column format (time, X, S) for plotting

### Near-term goals
2. **Extended simulation test**: Run longer simulations (100+ steps) to verify numerical stability with light coupling and self-shading
3. **Refactor validation to use constructor pattern**: Move `validateParameters` and `validateState` into constructors, consistent with `ReactorGeometry`

### Future enhancements
- Runge-Kutta integration (RK2 or RK4) for improved accuracy
- Adaptive time stepping
- Separate N and P tracking with dual nutrient limitation
- 1D spatial light profile (depth-dependent growth)
- Performance benchmarking
- Parallelization for parameter sweeps