#include "ReactorGeometry.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

ReactorGeometry::ReactorGeometry(QWidget* parent) : ParameterPanel(parent) {
    this->setTitle("Reactor Geometry");
    this->m_depth = this->addRow("d - Depth",                           "0.05", "m");
    this->m_I0    = this->addRow("I0 - Surface Irradiance",             "200", "µmol photons m⁻² s⁻¹");
    this->m_k     = this->addRow("k - Light Extinction Coefficient",    "0.2", "m⁻¹·(g/L)⁻¹");
}