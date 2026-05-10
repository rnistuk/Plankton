#pragma once
#include "ParameterPanel.h"

class SimulationControlsPanel : public ParameterPanel {
    Q_OBJECT
    QLineEdit* m_dt;
    QLineEdit* m_numSteps;
public:
    SimulationControlsPanel(QWidget* parent = nullptr, double dt = 0.01, int numSteps = 1000);
    double dt() const;
    int numSteps() const;
};
