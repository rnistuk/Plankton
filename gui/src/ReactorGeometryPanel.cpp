#include "ReactorGeometryPanel.h"

#include <QDoubleValidator>

ReactorGeometryPanel::ReactorGeometryPanel(QWidget* parent,
    double depth, double I0, double k) : ParameterPanel(parent) {
    this->setTitle("Reactor Geometry");
    this->m_depth = this->addRow("depth", "d - Depth"
        , QString::number(depth), "m"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_I0    = this->addRow("I0", "I0 - Surface Irradiance"
        , QString::number(I0) , "µmol photons m⁻² s⁻¹"
        , new QDoubleValidator(0, 1e9, 6, this));
    this->m_k     = this->addRow("k", "k - Light Extinction Coefficient"
        , QString::number(k), "m⁻¹·(g/L)⁻¹"
        , new QDoubleValidator(0, 1e9, 6, this));
}

ReactorGeometry ReactorGeometryPanel::toGeometry() const {
    return ReactorGeometry(this->m_depth->text().toDouble(), this->m_I0->text().toDouble(), this->m_k->text().toDouble());
}
