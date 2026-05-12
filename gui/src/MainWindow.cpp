#include "MainWindow.h"
#include "Simulation.h"
#include "BeerLambert.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)  : QMainWindow(parent) {
    this->setWindowTitle("Plankton Growth Simulator");
    this->root = new QWidget(this);
    this->controls = new SimulationControlsPanel();
    this->initial = new InitialConditionsPanel();
    this->kinetic = new KineticParametersPanel();
    this->reactor = new ReactorGeometryPanel();
    this->results = new Results();

    auto* inputLayout = new QVBoxLayout();
    inputLayout->addWidget(controls);
    inputLayout->addWidget(initial);
    inputLayout->addWidget(reactor);
    inputLayout->addWidget(kinetic);

    inputLayout->addStretch(1);
    inputLayout->setSpacing(25);

    auto* resultLayout = new QVBoxLayout();
    resultLayout->addWidget(results);

    auto* layout = new QHBoxLayout();
    layout->addLayout(inputLayout);
    layout->addLayout(resultLayout);

    layout->setStretch(0, 0);   // input column: don't grow
    layout->setStretch(1, 1);   // result column: claim all extra horizontal space

    root->setLayout(layout);
    setCentralWidget(root);
    this->resize(1600, 800);

    connect(controls, &ParameterPanel::parametersChanged, this, &MainWindow::runSimulation);
    connect(initial,  &ParameterPanel::parametersChanged, this, &MainWindow::runSimulation);
    connect(reactor,  &ParameterPanel::parametersChanged, this, &MainWindow::runSimulation);
    connect(kinetic,  &ParameterPanel::parametersChanged, this, &MainWindow::runSimulation);

    this->runSimulation();
}

void MainWindow::runSimulation() {
    const auto state = initial->toState();
    const auto kinetics = kinetic->toParameters();
    const auto geometry = reactor->toGeometry();
    const SimulationParameters params(kinetics, geometry,
                                      controls->dt(), controls->numSteps());
    const auto lightModel = [geometry](double X) {
        return depthAveragedIrradiance(geometry, X);
    };
    bool stop = false;
    results->setRecords(controls->dt(),
                        simulate(state, params, lightModel, stop));
}
