#include "KineticParameters.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>


KineticParameters::KineticParameters(QWidget* parent) : QGroupBox(parent) {
    this->setTitle("Kinetic Parameters");

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(new QLabel("Ks - Half-saturation"), 0, 0);
    layout->addWidget(new QLineEdit("0.05"), 0, 1);
    layout->addWidget(new QLabel("g/L"), 0, 2);

    layout->addWidget(new QLabel("\xce\xbc_max - Max growth rate"), 1, 0);
    layout->addWidget(new QLineEdit("0.05"), 1, 1);
    layout->addWidget(new QLabel("1/day"), 1, 2);

    layout->addWidget(new QLabel("Yx/s - Biomass yield"), 2, 0);
    layout->addWidget(new QLineEdit("0.05"), 2, 1);
    layout->addWidget(new QLabel("g/g"), 2, 2);

    layout->addWidget(new QLabel("Ki - Light half-saturation"), 3, 0);
    layout->addWidget(new QLineEdit("0.05"), 3, 1);
    layout->addWidget(new QLabel("\xc2\xb5mol/m\xc2\xb2/s"), 3, 2);

    layout->addWidget(new QLabel("kd - decay rate"), 4, 0);
    layout->addWidget(new QLineEdit("0.05"), 4, 1);
    layout->addWidget(new QLabel("1/day"), 4, 2);

    this->setLayout(layout);


}
