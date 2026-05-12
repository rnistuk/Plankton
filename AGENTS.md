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
  - Signature: `simulate(const MonodState&, const SimulationParameters&, const LightModel&, bool& stop)` returns `vector<SimulationRecord>`
  - `bool& stop` is an out-parameter the caller sets to `true` to interrupt a running simulation mid-flight
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


### 🚧 In progress — Qt6 Dashboard GUI (`gui/`)

- **Technology**: Qt6 (Widgets, Charts); `gui/` is a CMake target that links against `plankton_lib`
- **Panel layout**: left column of input `QGroupBox` panels → right column with live chart
  - `InitialConditionsPanel` — X (biomass), S (substrate); maps to `MonodState`
  - `ReactorGeometryPanel` — depth, I₀, k; maps to `ReactorGeometry` struct
  - `KineticParametersPanel` — Ks, µ_max, Yx/s, Ki, kd; maps to `MonodParameters`
  - `SimulationControlsPanel` — dt, numSteps; maps to `SimulationParameters::dt` and `numSteps`
  - `Results` — `QChartView` displaying X and S time series
- **GUI test target**: `GuiTests` is a separate CMake executable from `PlanktonTests`, with its own `gui/test_main.cpp` that constructs a `QApplication` before `RUN_ALL_TESTS()`. Keeps `plankton_lib` and `PlanktonTests` Qt-free.
- **Test access seam for panels**: `ParameterPanel::addRow(name, label, value, units)` calls `setObjectName(name)` on the `QLineEdit` it constructs; tests reach in via `panel.findChild<QLineEdit*>("X")` to mutate text and verify accessors read live values. This avoids exposing private members and is good Qt hygiene independent of testing. (`addRow` originally took only `(label, value, units)`; the `name` parameter was hoisted to the front during refactor — separates "what this row *is*" from "what this row *shows*".)
- **Status**: all four input panels are fully wired. `MainWindow` orchestration is complete: panels are members, `runSimulation()` slot reads all panels, builds `SimulationParameters`, injects the Beer-Lambert lambda, calls `simulate()`, hands records to `Results::setRecords()`, and is triggered once at the end of the ctor for the initial render. `Results` series refactor is complete: `xSeries`/`sSeries` are members initialised once in the ctor with object names `"X"`/`"S"`; `createDefaultAxes()` is called once in the ctor with axis titles; `setRecords()` uses `clear()`+`append()` and explicitly updates axis ranges (horizontal: `[0, t_max]`; vertical: `[0, maxConc * 1.01]`). Fourteen GUI tests in `GuiTests` total, including `SetRecordsKeepsSeriesPointers` (verifies pointer stability across two `setRecords` calls) and a `findSeriesInChartViewNamed` helper. Next phase: live-update wiring (steps A and B — `parametersChanged()` signals + validators).

#### Live-update wiring plan

Goal: when the user edits any parameter, the chart re-renders automatically.

**Signal flow:**
```
QLineEdit::editingFinished
        │
        ▼
panel emits parametersChanged()
        │
        ▼
MainWindow::runSimulation()  ← gathers values from all 3 panels
        │
        ▼
simulate(...) returns vector<SimulationRecord>
        │
        ▼
Results::setRecords(records)  ← clears & repopulates series
```

**Why `editingFinished` over `textChanged`:** `textChanged` fires per keystroke and would re-run the simulation on partial input like `"1."`. `editingFinished` fires once on Enter or focus loss, and only when the validator considers the input acceptable.

**Why panel-forwards-signal over MainWindow-connects-directly-to-QLineEdits:** the panel keeps its internal layout private from `MainWindow`, and can later debounce, validate, or only emit when `hasAcceptableInput()` is true.

**Step order (each step compiles and is small):**

- **A — `parametersChanged()` signal on each panel.** Add `signals: void parametersChanged();`. In each panel ctor, connect every member `QLineEdit::editingFinished` → `parametersChanged`. Consider a helper on `ParameterPanel` so newly-added rows wire up automatically.
- **B — Validators.** `QDoubleValidator` on doubles (X, S, dt, Ks, µ_max, Yx_s, Ki, kd, depth, I0, k) with `bottom = 0` where appropriate; `QIntValidator` on `numSteps`. Required for `editingFinished` to behave well.
- **C — Typed accessors on each panel.** ✅ `MonodState toState()` on `InitialConditionsPanel`; ✅ `ReactorGeometry toGeometry()` on `ReactorGeometryPanel`; ✅ `MonodParameters toParameters()` on `KineticParametersPanel`; ✅ `double dt()` and `std::size_t numSteps()` on `SimulationControlsPanel`. Each panel locked down by three tests (defaults, ctor-args, live-read).
- **D — ✅ Promote panels to `MainWindow` members.** Done — all four panels and `Results` are members; `runSimulation()` reads them directly.
- **E — ✅ `MainWindow::runSimulation()` slot.** Done — reads all four panels, builds `SimulationParameters`, injects Beer-Lambert lambda, calls `simulate()`, hands records to `Results::setRecords()`. Initial render triggered at end of ctor.
- **F — ✅ `Results::setRecords(double dt, const std::vector<SimulationRecord>&)`.** Done — refactored from remove+recreate to member series with `clear()`+`append()`. `xSeries`/`sSeries` are members initialised in ctor with object names `"X"`/`"S"`. `createDefaultAxes()` called once in ctor with axis titles. `setRecords()` updates axis ranges explicitly: horizontal `[0, t_max]`, vertical `[0, maxConc * 1.01]`. Locked down by `SetRecordsPopulatesSeries` and `SetRecordsKeepsSeriesPointers`; `findSeriesInChartViewNamed` helper introduced.

**Threading:** first pass runs synchronously on the GUI thread — `simulate()` for ~1000 steps is fast enough. Move to a worker (with `std::stop_token` instead of `bool&`) only if/when the chart updates feel sluggish.

### ❌ Not started
- **Separate N and P tracking**: Break out nitrogen and phosphorus as separate state variables instead of generic substrate S
  - Update `MonodState` to include N and P fields
  - Update `MonodParameters` to include separate Ks_N, Ks_P, Yx_N, Yx_P
  - Implement dual nutrient limitation (Liebig's law of the minimum)
  - Update main.cpp output to show t, X, N, P
  - Update tests for new structure
- **Generic integration refactoring + RK2/RK4** *(near-term)*: Make `eulerStep` model-agnostic so any integration method (Euler, RK2, RK4) can be swapped in without touching the model.
  - **Open decision (resolve before implementing):** what type represents the ODE state vector? Candidates: `std::vector<double>` (flexible, heap-allocated) or `std::array<double, N>` (stack, fixed-size). This choice determines the signatures of both the integrator and the derivative function.
  - **Integrator signature:** `eulerStep(state, dt, derivative_fn)` — takes a state vector and a callable, returns a new state vector of the same type. Knows nothing about biology.
  - **Monod derivative function** (name TBD, e.g. `monodDerivatives`): a pure function `(state, I_avg, MonodParameters) → derivatives` capturing all kinetics and stoichiometry. Lives in `Monod.h`/`Monod.cpp`. The caller in `simulate()` wraps it in a lambda that closes over the current step's `I_avg` and passes it to the integrator.
  - **`simulate()` change:** builds the derivative lambda, calls the generic integrator, clamps to zero, and constructs `SimulationRecord`. Non-negativity enforcement moves here — resolves the `MonodState` type invariant issue (the current copy-then-mutate in `eulerStep` can produce negative values that violate the constructor's guarantee).
  - **File changes:** `eulerStep` leaves `Monod.h`/`Monod.cpp` and moves to a new header (e.g. `Integration.h`) as a function template (implementation stays in the header; no new `.cpp` needed). `Monod.h` gains the new derivative function.
  - **Swap to RK2/RK4:** once the derivative function exists, adding RK2/RK4 is a new integrator function with the same signature — no model changes needed.
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
- Dead biomass does not recycle substrate: the `kd` mortality term reduces X but does not return nutrients to S

## Next steps

### Future enhancements
- **Template `simulate()`**: Replace `std::function<double(double)>` `LightModel` with a template parameter to eliminate `std::function` overhead while preserving the injectable light model design
- **RK2/RK4 integration**: see *Generic integration refactoring* in ❌ Not started — the derivative function extracted there enables this directly
- **Adaptive time stepping**
- **Separate N and P tracking**: Replace generic substrate `S` with distinct nitrogen and phosphorus state variables; dual nutrient limitation via Liebig's Law
  - `MonodState`: add N and P fields
  - `MonodParameters`: add Ks_N, Ks_P, Yx_N, Yx_P
  - Update `cli/` output to show t, X, N, P
- **1D spatial light profile**: Extend from depth-averaged scalar to PDE
- **Performance benchmarking and parallelization** for parameter sweeps