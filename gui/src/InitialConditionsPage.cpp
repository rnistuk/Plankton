#include "InitialConditionsPage.h"
#include <QFormLayout>

InitialConditionsPage::InitialConditionsPage(QWidget *parent)
    : PlanktonWizardPage(parent)
{
    setTitle("Initial Conditions");

    m_xEdit        = new QDoubleSpinBox;
    m_sEdit        = new QDoubleSpinBox;
    m_dtEdit       = new QDoubleSpinBox;
    m_numStepsEdit = new QSpinBox;
    m_durationLabel = new QLabel;

    // configure ranges and defaults
    m_xEdit->setRange(0.001, 10.0);    m_xEdit->setValue(0.05);   m_xEdit->setDecimals(4);
    m_sEdit->setRange(0.01,  5.0);     m_sEdit->setValue(5.0);    m_sEdit->setDecimals(4);
    m_dtEdit->setRange(0.001, 0.1);    m_dtEdit->setValue(0.01);  m_dtEdit->setDecimals(4);
    m_numStepsEdit->setRange(10, 10000); m_numStepsEdit->setValue(1000);

    registerField("initialConditions.X",        m_xEdit,        "value");
    registerField("initialConditions.S",        m_sEdit,        "value");
    registerField("simulation.dt",              m_dtEdit,       "value");
    registerField("simulation.numSteps",        m_numStepsEdit, "value");

    connect(m_dtEdit,       QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &InitialConditionsPage::updateDurationLabel);
    connect(m_numStepsEdit, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &InitialConditionsPage::updateDurationLabel);

    // layout -- two-column form
    auto *layout = new QFormLayout;
    layout->addRow("Initial Biomass X (g/L):",  m_xEdit);
    layout->addRow("Initial Substrate S (g/L):", m_sEdit);
    layout->addRow("Time Step dt (days):",       m_dtEdit);
    layout->addRow("Number of Steps:",           m_numStepsEdit);
    layout->addRow("Total Duration:",            m_durationLabel);
    setLayout(layout);

    updateDurationLabel();
}

void InitialConditionsPage::updateDurationLabel() {
    double duration = m_dtEdit->value() * m_numStepsEdit->value();
    m_durationLabel->setText(QString::number(duration, 'f', 2) + " days");
}

bool InitialConditionsPage::validateInputs() { return true; }
void InitialConditionsPage::populateFromFields() {}