#include "SimulationControlsPanel.h"
#include <QIntValidator>
#include <QDoubleValidator>

SimulationControlsPanel::SimulationControlsPanel(QWidget* parent, double dt, size_t numSteps) : ParameterPanel(parent) {
    this->setTitle("Simulation Controls");
    this->m_dt  = this->addRow("dt", "dt - Time Step"
        , QString::number(dt), "days"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_numSteps  = this->addRow("N", "N - Number of Steps", QString::number(numSteps), ""
        , new QIntValidator(1, 100000, this));
}

double SimulationControlsPanel::dt() const {
    return this->m_dt->text().toDouble();
}

size_t SimulationControlsPanel::numSteps() const {
    return this->m_numSteps->text().toULongLong();
}
