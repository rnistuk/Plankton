#pragma once
#include "PlanktonWizardPage.h"
#include <QDoubleSpinBox>

class KineticParametersPage : public PlanktonWizardPage {
    Q_OBJECT
public:
    explicit KineticParametersPage(QWidget *parent = nullptr);
    bool validateInputs() override;
    void populateFromFields() override;

private:
    QDoubleSpinBox *m_ksEdit;
    QDoubleSpinBox *m_muMaxEdit;
    QDoubleSpinBox *m_yxsEdit;
    QDoubleSpinBox *m_kiEdit;
    QDoubleSpinBox *m_kdEdit;
};
