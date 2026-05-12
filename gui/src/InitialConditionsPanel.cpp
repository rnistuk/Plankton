#include "InitialConditionsPanel.h"

#include <QDoubleValidator>

InitialConditionsPanel::InitialConditionsPanel(QWidget* parent, double X, double S) : ParameterPanel(parent) {
    this->setTitle("Initial Conditions");
    this->m_X  = this->addRow("X", "X - Biomass"
        , QString::number(X), "g/L"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_S  = this->addRow("S", "S - Substrate"
        ,QString::number(S), "g/L"
        , new QDoubleValidator(0, 1e9, 6, this));
}

MonodState InitialConditionsPanel::toState() const {
    return MonodState(this->m_X->text().toDouble(), this->m_S->text().toDouble());
}
