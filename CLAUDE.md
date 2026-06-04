Plankton — CLAUDE.md
============================

A C++23 implementation of a coupled Monod-kinetics + Beer-Lambert light-attenuation
model for algae bioreactor growth, built with strict TDD. It models phytoplankton
biomass dynamics (biomass X, substrate S, depth-averaged light I) via the Monod equation
and numerical integration, exports CSV time series, and offers a Qt6 dashboard GUI.
Intended as a portfolio piece targeting Industrial Plankton (Metchosin/Langford).

Project Structure
-----------------

Three CMake subprojects under the root, all C++23:

*   `plankton/` — the core static library `plankton_lib` (the simulation). `inc/` public
    + internal headers, `src/` implementation, `test_main.cpp` the `PlanktonTests` runner.
    Start here.
    *   Public headers: `Simulation.h`, `SimulationParameters.h`, `MonodParameters.h`,
        `MonodState.h`, `ReactorGeometry.h`, `SimulationRecord.h`, `CsvExport.h`.
    *   Internal: `Monod.h` (kinetics + integration), `BeerLambert.h` (light).
    *   `Simulation.cpp` owns `simulate()`; `Monod.cpp` owns kinetics/integration only.
*   `cli/` — `main.cpp`, a demo program: a 1000-step simulation with realistic
    phytoplankton parameters, CSV to stdout. Links `plankton_lib`.
*   `gui/` — Qt6 (Widgets, Charts) dashboard. Input panels on the left, live chart on the
    right. `test_main.cpp` is the separate `GuiTests` runner (constructs a `QApplication`
    before `RUN_ALL_TESTS()`), which keeps `plankton_lib`/`PlanktonTests` Qt-free.
*   `Resources/` — assets. `cmake-build-debug*/` — CLion build dirs (not source).

Environment
-----------

*   IDE: CLion (recommended)
*   Language: C++23
*   Build system: CMake 4.1+ (root sets `CMAKE_CXX_STANDARD 23`)
*   Key dependencies: Google Test v1.15.2 (fetched via `FetchContent`); Qt6 (Widgets,
    Charts) — required for the `gui/` target only. GCC 13+, Clang 16+, or MSVC 2022+.

Building and Testing
--------------------

```
cmake -B build -S .
cmake --build build
cd build && ctest --output-on-failure
```

Two test executables: `PlanktonTests` (the Qt-free core library) and `GuiTests` (Qt;
own `QApplication`). Run the demo with the `cli` target's binary to emit CSV. The Qt-free
split is deliberate — keep core logic and its tests independent of Qt.

Conventions
-----------

*   TDD — write failing tests first, then minimum code to pass (Red → Green → Refactor),
    Google Test. Test pure functions first (Monod), then integration (`eulerStep`), then
    the simulation loop.
*   No comments unless the why is non-obvious
*   No speculative code — only implement what is needed right now
*   Commit messages: short, descriptive, imperative tense ("add Beer-Lambert attenuation" not "added" or "adding")
*   Functional style: pure functions + immutable data; domain constraints enforced at
    construction (all structs validate in their constructors — invalid objects are
    unconstructable); low-level functions (`eulerStep`) trust their inputs.
*   Keep domain logic (kinetics) separate from orchestration (the simulation loop), and
    keep `plankton_lib` free of Qt.
*   Dependency inversion for the light model: `simulate()` takes a `LightModel`
    (`std::function<double(double)>`); Beer-Lambert is injected via lambda at call sites.
*   Use tolerance-based assertions for floating-point tests; export results CSV-first.
*   Standing design decisions: `dt` lives in `SimulationParameters` (numerical, not
    biological); separate State (variables) from Parameters (constants); explicit yield
    coefficient `Y_x/s` couples growth and substrate consumption.
*   Standing assumptions: depth-averaged light (no spatial gradients, well-mixed reactor);
    optimal temperature/pH (not modelled); dead biomass (`kd`) does not recycle substrate.

Current State
-------------

### Completed

*   **Data structures** — `MonodState` (X, S; validated), `MonodParameters` (Ks, µ_max,
    Yx_s, Ki, kd; validated), `SimulationParameters` (aggregates params + geometry + dt,
    validates dt > 0), `SimulationRecord` (X, S, depth-averaged I_avg per step),
    `ReactorGeometry` (depth, I0, k; validated).
*   **Euler integration** (`eulerStep(state, params, I_avg, dt)`) — single step;
    `dX/dt = (µ − kd)·X`; substrate consumption coupled via yield; mass balance verified.
*   **Multi-step simulation** (`simulate(state, params, LightModel, bool& stop)`) —
    returns `vector<SimulationRecord>` (initial + N steps); `stop` out-param interrupts
    mid-flight; clamps X, S ≥ 0. Tests cover growth/depletion, non-negativity, depth
    effect, I_avg recording.
*   **Beer-Lambert** — `beerLambert(z, geometry, X)` = I₀·exp(−k·X·z);
    `depthAveragedIrradiance(geometry, X)` with a near-zero-biomass guard (k·X·depth <
    1e-15 → I₀) to avoid NaN from denormals.
*   **Light-limited growth** (`lightLimitedGrowthRate`) — Liebig's Law:
    µ = µ_max·min(S/(Ks+S), I_avg/(Ki+I_avg)); `eulerStep` requires explicit `I_avg`.
*   **Mortality** (`kd`), **CSV export** (`writeCsv`), **demo** (`cli/main.cpp`),
    **module separation**, **public/internal header split**, **`plankton_lib` static lib**,
    and **CMake** (sources-only list; tests link the lib; Google Test integration).

### In Progress

*   **Qt6 dashboard GUI** (`gui/`) — four input panels (InitialConditions, ReactorGeometry,
    KineticParameters, SimulationControls) fully wired; `MainWindow` orchestration complete;
    `Results` `QChartView` series refactor complete; live-update wiring complete
    (`parametersChanged()` on `ParameterPanel`, connected in `MainWindow`;
    `QDoubleValidator`/`QIntValidator` on inputs; Run button removed). Editing any field
    re-runs the sim on `editingFinished` and re-renders. 14 GUI tests. Runs synchronously
    on the GUI thread (fast enough at ~1000 steps). Next phase: generic integrator refactor.

### Next

*   **Generic integration refactor + RK2/RK4** — make `eulerStep` model-agnostic:
    `eulerStep(state, dt, derivative_fn)` returning a new state; extract a pure Monod
    derivative function; move non-negativity enforcement into `simulate()`. Once the
    derivative function exists, RK2/RK4 are drop-in integrators with the same signature.
*   **Template `simulate()`** — replace the `std::function` `LightModel` with a template
    parameter to drop the indirection while keeping injectability.
*   **Separate N and P tracking** — split substrate S into nitrogen + phosphorus state
    vars; add Ks_N/Ks_P, Yx_N/Yx_P; dual nutrient limitation (Liebig); update CLI output.
*   **1D spatial light profile** (depth-averaged → PDE); adaptive time stepping;
    performance benchmarking/parallelization for parameter sweeps.

### Decisions Pending

*   **ODE state-vector type for the generic integrator** — `std::vector<double>` (flexible,
    heap) vs `std::array<double, N>` (stack, fixed-size). This choice sets the signatures
    of both the integrator and the derivative function; resolve before implementing the
    refactor.

Do Not Touch
------------

*   Nothing off limits at this time. (Keep `plankton_lib` and `PlanktonTests` Qt-free —
    Qt belongs only in `gui/` — because the Qt-free split is what keeps the core library
    portable and fast to test.)

Pair Programming
----------------

*   Driver writes all code — navigator (Claude) does not write code unless explicitly asked
*   Navigator provides: direction, design decisions, code review, and course corrections
*   One step at a time — navigator waits for driver to confirm before moving to the next
*   Navigator confirms tests pass before suggesting the next step
*   Navigation is terse — no lengthy explanations unless explicitly asked
*   If navigator disagrees with an approach, say so once then follow the driver's lead

Karpathy Rules
--------------

All rules apply to every task unless explicitly overridden.

### Rule 1 — Think Before Coding

State assumptions explicitly. If uncertain, ask rather than guess. Present multiple interpretations when ambiguity exists. Push back when a simpler approach exists. Stop when confused. Name what's unclear.

### Rule 2 — Simplicity First

Minimum code that solves the problem. Nothing speculative. No features beyond what was asked. No abstractions for single-use code. Test: would a senior engineer say this is overcomplicated? If yes, simplify.

### Rule 3 — Surgical Changes

The driver should touch only what is necessary to make the current test pass. Don't "improve" adjacent code, comments, or formatting. Don't refactor what isn't broken. Match existing style. Navigator warns the driver if this rule is being broken.

### Rule 4 — Agree on Done Before Starting

Before implementing anything, both driver and navigator should agree on what done looks like. A failing test is the preferred success criterion. Don't start until done is defined.

### Rule 5 — Dropped

Original rule was written for agentic coding and does not apply to this pair programming model.

### Rule 6 — Know When to Stop

If the session is getting long or context feels stale, checkpoint before continuing. Summarise what's done, what's verified, what's next. Update CLAUDE.md to reflect current state, then commit before ending the session. A fresh session with good context beats a stale one with accumulated confusion.

### Rule 7 — Surface Conflicts, Don't Average Them

If two patterns contradict, pick one (more recent / more tested). Explain why. Flag the other for cleanup. Don't blend conflicting patterns.

### Rule 8 — Read Before You Write

Before adding code, read exports, immediate callers, shared utilities. "Looks orthogonal" is dangerous. If unsure why code is structured a way, ask.

### Rule 9 — Test Behaviours, Not Functionality

Tests verify what the code does from the outside, not how it does it internally. Tests must encode WHY the behaviour matters, not just WHAT it does. A test that can't fail when business logic changes is wrong.

### Rule 10 — Checkpoint After Every Significant Step

Summarize what was done, what's verified, what's left. Don't continue from a state you can't describe back. If you lose track, stop and restate.

### Rule 11 — Match the Codebase's Conventions, Even If You Disagree

Conformance > taste inside the codebase. If you genuinely think a convention is harmful, surface it. Don't fork silently.

### Rule 12 — Fail Loud

"Completed" is wrong if anything was skipped silently. "Tests pass" is wrong if any were skipped. Default to surfacing uncertainty, not hiding it.
