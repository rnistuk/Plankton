#pragma once
#include "MonodState.h"
#include "ParameterPanel.h"

class InitialConditionsPanel : public ParameterPanel {
    Q_OBJECT
    QLineEdit* m_X;
    QLineEdit* m_S;
public:
    InitialConditionsPanel(QWidget* parent = nullptr
        , double X = 2.0
        , double S = 2.0);
    MonodState toState() const;
};
