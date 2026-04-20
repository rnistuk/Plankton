#pragma once
#include <QWizardPage>

class PlanktonWizardPage : public QWizardPage {
    Q_OBJECT
public:
    explicit PlanktonWizardPage(QWidget *parent = nullptr) : QWizardPage(parent) {}
    bool validatePage() override { return validateInputs(); }
    virtual bool validateInputs() = 0;
    virtual void populateFromFields() = 0;
};
