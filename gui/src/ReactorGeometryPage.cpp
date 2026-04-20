#include "ReactorGeometryPage.h"

#include <QFormLayout>
#include <QLabel>

ReactorGeometryPage::ReactorGeometryPage(QWidget* parent)
    : PlanktonWizardPage(parent) {
    setTitle("Reactor Geometry");
    m_depthEdit     = new QDoubleSpinBox;
    m_I0Edit        = new QDoubleSpinBox;
    m_kEdit         = new QDoubleSpinBox;

    m_depthEdit->setRange(0.01, 10.0);
    m_I0Edit->setRange(1.0, 2000.0);
    m_kEdit->setRange(0.01, 10.0);

    m_depthEdit->setValue(0.05);
    m_I0Edit->setValue(200.0);
    m_kEdit->setValue(0.2);

    registerField("reactor.depth", m_depthEdit, "value");
    registerField("reactor.I0", m_I0Edit, "value");
    registerField("reactor.k", m_kEdit, "value");

    auto *layout = new QFormLayout;
    layout->addRow("Reactor depth m:", m_depthEdit);
    layout->addRow("Surface Irradiance µmol photons m⁻² s⁻¹:", m_I0Edit);
    layout->addRow("light extinction coefficient:", m_kEdit);

    setLayout(layout);
}

bool ReactorGeometryPage::validateInputs() { return true; }

void ReactorGeometryPage::populateFromFields() { }
