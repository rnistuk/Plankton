#pragma once
#include "PlanktonWizardPage.h"
#include <QWidget>
#include <QLabel>
#include <QSpinBox>

// Fields: X, S, dt, num_steps. Live-calculated "Total Duration" label (dt x num_steps).
class InitialConditionsPage : public PlanktonWizardPage {
    Q_OBJECT
public:
    explicit InitialConditionsPage(QWidget *parent = nullptr);
    bool validateInputs() override;
    void populateFromFields() override;

private slots:
    void updateDurationLabel();

private:
    QDoubleSpinBox *m_xEdit;
    QDoubleSpinBox *m_sEdit;
    QDoubleSpinBox *m_dtEdit;
    QSpinBox       *m_numStepsEdit;
    QLabel         *m_durationLabel;
};
