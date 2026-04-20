#pragma once
#include "PlanktonWizardPage.h"
#include <QDoubleSpinBox>

class ReactorGeometryPage : public PlanktonWizardPage {
    Q_OBJECT
public:
    explicit ReactorGeometryPage(QWidget* parent = nullptr);
    bool validateInputs() override;
    void populateFromFields() override;

private:
    QDoubleSpinBox *m_depthEdit;
    QDoubleSpinBox *m_I0Edit;
    QDoubleSpinBox *m_kEdit;
};
