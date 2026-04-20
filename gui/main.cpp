#include "PlanktonWizard.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PlanktonWizard wizard;
    wizard.show();
    return app.exec();
}