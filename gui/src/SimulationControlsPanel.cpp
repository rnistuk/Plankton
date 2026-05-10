#include "SimulationControlsPanel.h"

SimulationControlsPanel::SimulationControlsPanel(QWidget* parent, double dt, int numSteps) : ParameterPanel(parent) {
    this->setTitle("Simulation Controls");
    this->m_dt  = this->addRow("dt", "dt - Time Step", QString::number(dt), "days");
    this->m_numSteps  = this->addRow("N", "N - Number of Steps", QString::number(numSteps), "");
}

double SimulationControlsPanel::dt() const {
    return this->m_dt->text().toDouble();
}

int SimulationControlsPanel::numSteps() const {
    return this->m_numSteps->text().toInt();
}

