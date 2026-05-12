#include "KineticParametersPanel.h"

#include <QDoubleValidator>

KineticParametersPanel::KineticParametersPanel(QWidget* parent, double Ks, double muMax, double Yxs, double Ki, double kd) : ParameterPanel(parent) {
    this->setTitle("Kinetic Parameters");
    this->m_Ks    = addRow("Ks", "Ks - Half-saturation"
        ,QString::number(Ks),"g/L"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_muMax = addRow("muMax", "µ_max - Max growth rate",    QString::number(muMax),   "1/day"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_Yxs   = addRow("Yxs", "Yx/s - Biomass yield",       QString::number(Yxs),   "g/g"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_Ki    = addRow("Ki", "Ki - Light half-saturation", QString::number(Ki), "µmol/m²/s"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_kd    = addRow("kd", "kd - Decay rate",            QString::number(kd),   "1/day"
        , new QDoubleValidator(0, 1e9, 6, this));
}

MonodParameters KineticParametersPanel::toParameters() const {
    return MonodParameters(this->m_Ks->text().toDouble(), this->m_muMax->text().toDouble(), this->m_Yxs->text().toDouble(), this->m_Ki->text().toDouble(), this->m_kd->text().toDouble());
}
