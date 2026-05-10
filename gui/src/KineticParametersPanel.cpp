#include "KineticParametersPanel.h"

KineticParametersPanel::KineticParametersPanel(QWidget* parent, double Ks, double muMax, double Yxs, double Ki, double kd) : ParameterPanel(parent) {
    this->setTitle("Kinetic Parameters");
    this->m_Ks    = addRow("Ks", "Ks - Half-saturation",       QString::number(Ks),   "g/L");
    this->m_muMax = addRow("muMax", "µ_max - Max growth rate",    QString::number(muMax),   "1/day");
    this->m_Yxs   = addRow("Yxs", "Yx/s - Biomass yield",       QString::number(Yxs),   "g/g");
    this->m_Ki    = addRow("Ki", "Ki - Light half-saturation", QString::number(Ki), "µmol/m²/s");
    this->m_kd    = addRow("kd", "kd - Decay rate",            QString::number(kd),   "1/day");
}

MonodParameters KineticParametersPanel::toParameters() const {
    return MonodParameters(this->m_Ks->text().toDouble(), this->m_muMax->text().toDouble(), this->m_Yxs->text().toDouble(), this->m_Ki->text().toDouble(), this->m_kd->text().toDouble());
}
