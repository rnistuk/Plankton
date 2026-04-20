#include "KineticParametersPage.h"
#include <QFormLayout>

KineticParametersPage::KineticParametersPage(QWidget *parent)
    : PlanktonWizardPage(parent)
{
    setTitle("Kinetic Parameters");

    m_ksEdit    = new QDoubleSpinBox;
    m_muMaxEdit = new QDoubleSpinBox;
    m_yxsEdit   = new QDoubleSpinBox;
    m_kiEdit    = new QDoubleSpinBox;
    m_kdEdit    = new QDoubleSpinBox;

    m_ksEdit->setRange(0.001, 10.0);
    m_ksEdit->setValue(1.0);
    m_ksEdit->setDecimals(4);

    m_muMaxEdit->setRange(0.001, 10.0);
    m_muMaxEdit->setValue(1.5);
    m_muMaxEdit->setDecimals(4);

    m_yxsEdit->setRange(0.001, 10.0);
    m_yxsEdit->setValue(6.6);
    m_yxsEdit->setDecimals(4);

    m_kiEdit->setRange(0.1, 10000.0);
    m_kiEdit->setValue(100.0);
    m_kiEdit->setDecimals(2);

    m_kdEdit->setRange(0.0, 10.0);
    m_kdEdit->setValue(0.3);
    m_kdEdit->setDecimals(4);

    registerField("kinetics.Ks",     m_ksEdit,    "value");
    registerField("kinetics.mu_max", m_muMaxEdit, "value");
    registerField("kinetics.Yx_s",   m_yxsEdit,   "value");
    registerField("kinetics.Ki",     m_kiEdit,    "value");
    registerField("kinetics.kd",     m_kdEdit,    "value");

    auto *layout = new QFormLayout;
    layout->addRow("Half-saturation Ks (g/L):",           m_ksEdit);
    layout->addRow("Max growth rate \xce\xbc_max (1/day):", m_muMaxEdit);
    layout->addRow("Biomass yield Yx/s (g/g):",           m_yxsEdit);
    layout->addRow("Light half-sat Ki (\xc2\xb5mol/m\xc2\xb2/s):", m_kiEdit);
    layout->addRow("Decay rate kd (1/day):",              m_kdEdit);
    setLayout(layout);
}

bool KineticParametersPage::validateInputs() { return true; }
void KineticParametersPage::populateFromFields() {}
