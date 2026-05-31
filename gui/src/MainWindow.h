#pragma once
#include "InitialConditionsPanel.h"
#include "KineticParametersPanel.h"
#include "ReactorGeometryPanel.h"
#include "Results.h"
#include "SimulationControlsPanel.h"

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
    QWidget* root;
    InitialConditionsPanel*   initial;
    KineticParametersPanel*   kinetic;
    ReactorGeometryPanel*     reactor;
    Results*                  results;
    SimulationControlsPanel*  controls;

public:
    MainWindow(QWidget *parent = nullptr);
    void runSimulation();
};
