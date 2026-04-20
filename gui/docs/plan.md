# GUI Design Plan

## Context

The plankton simulation has a working core library (`plankton_lib`) and a CLI. A GUI stub exists in `gui/main.cpp` using Qt6::Widgets. This plan replaces the stub with a multi-page wizard GUI using C++ OOP (classes, inheritance, polymorphism).

---

## Class Design

```
QWizardPage
    PlanktonWizardPage  (abstract base)
        + validatePage() override -> calls validateInputs()
        + virtual bool validateInputs() = 0
        + virtual void populateFromFields() = 0
        |
        +-- InitialConditionsPage
        +-- KineticParametersPage
        +-- ReactorGeometryPage
        +-- ResultsPage

QWizard
    PlanktonWizard
        owns all four pages
        assembles SimulationParameters from field map
        calls simulate()
        passes results vector to ResultsPage
```

`PlanktonWizardPage` overrides `QWizardPage::validatePage()` to dispatch to the pure virtual `validateInputs()`. Each page subclass implements its own validation without touching the others.

---

## Data Flow: QWizard Field Registration

Pages register inputs with `registerField("fieldName", widget)`. `PlanktonWizard` reads all values after the last page via `field("fieldName").toDouble()` and assembles the typed model structs.

Field name conventions:

| Field               | Key                       |
|---------------------|---------------------------|
| Initial biomass     | `initialConditions.X`     |
| Initial substrate   | `initialConditions.S`     |
| Time step           | `simulation.dt`           |
| Number of steps     | `simulation.numSteps`     |
| Max growth rate     | `kinetics.muMax`          |
| Substrate half-sat  | `kinetics.Ks`             |
| Biomass yield       | `kinetics.Yxs`            |
| Light half-sat      | `kinetics.Ki`             |
| Decay rate          | `kinetics.kd`             |
| Reactor depth       | `reactor.depth`           |
| Surface irradiance  | `reactor.I0`              |
| Extinction coeff    | `reactor.k`               |

---

## Pages

### Page 1: InitialConditionsPage
Fields: X, S, dt, num_steps. Live-calculated "Total Duration" label (dt x num_steps).

### Page 2: KineticParametersPage
Fields: mu_max, Ks, Yx_s, Ki, kd. Maps to MonodParameters.

### Page 3: ReactorGeometryPage
Fields: depth, I0, k. Maps to ReactorGeometry. Optional static Beer-Lambert diagram.

### Page 4: ResultsPage
`initializePage()` collects all fields, assembles SimulationParameters, calls `simulate()`.
Qt6::Charts QLineSeries for X, S, I_avg. Checkboxes toggle series visibility.
Export CSV button calls `writeCsv()`.

---

## Files

| File                        | Action  |
|-----------------------------|---------|
| `gui/main.cpp`              | Replace |
| `gui/PlanktonWizard.h/.cpp` | Create  |
| `gui/PlanktonWizardPage.h`  | Create  |
| `gui/InitialConditionsPage.h/.cpp` | Create |
| `gui/KineticParametersPage.h/.cpp` | Create |
| `gui/ReactorGeometryPage.h/.cpp`   | Create |
| `gui/ResultsPage.h/.cpp`   | Create  |
| `gui/CMakeLists.txt`        | Modify  |

---

## Build Changes

`gui/CMakeLists.txt` additions:
- `find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Charts)`
- Add `Qt6::Charts` to `target_link_libraries`
- List all new `.cpp` files in `qt_add_executable`

---

## Verification

1. `cmake --build build --target gui`
2. Run `./build/gui/gui`
3. Verify total duration label on page 1 updates when dt or num_steps change.
4. Enter a negative value; verify Next is blocked.
5. Complete all pages; verify results chart shows three series.
6. Click Export CSV; verify output matches `writeCsv()` format.
