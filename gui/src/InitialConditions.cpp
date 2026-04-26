#include "InitialConditions.h"

InitialConditions::InitialConditions(QWidget* parent) : ParameterPanel(parent) {
    this->setTitle("Initial Conditions");
    this->addRow("X - Biomass", "2", "g/L");
    this->addRow("S - Substrate", "2", "g/L");
    this->addRow("dt - Time Step", "0.01", "days");
    this->addRow("N - Number of Steps", "1000", "");
}