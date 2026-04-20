#include "PlanktonWizard.h"
#include "InitialConditionsPage.h"
#include "KineticParametersPage.h"
#include "ReactorGeometryPage.h"
#include "ResultsPage.h"

PlanktonWizard::PlanktonWizard(QWidget *parent) : QWizard(parent) {
    setWindowTitle("Plankton Growth Simulator");
    addPage(new InitialConditionsPage);
    addPage(new KineticParametersPage);
    addPage(new ReactorGeometryPage);
    addPage(new ResultsPage);
}
