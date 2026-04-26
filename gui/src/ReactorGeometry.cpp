#include "ReactorGeometry.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

ReactorGeometry::ReactorGeometry(QWidget* parent)  : QGroupBox(parent) {
    this->setTitle("Reactor Geometry");
    QGridLayout *layout = new QGridLayout;

    layout->addWidget(new QLabel("d - Depth"), 0, 0);
    layout->addWidget(new QLineEdit("0.05"), 0, 1);
    layout->addWidget(new QLabel("m"), 0, 2);

    layout->addWidget(new QLabel("I0 - Surface Irradiance"), 1, 0);
    layout->addWidget(new QLineEdit("0.05"), 1, 1);
    layout->addWidget(new QLabel("µmol photons m⁻² s⁻¹"), 1, 2);

    layout->addWidget(new QLabel("k - Light Extinction Coefficient"), 2, 0);
    layout->addWidget(new QLineEdit("0.05"), 2, 1);
    layout->addWidget(new QLabel("m⁻¹·(g/L)⁻¹"), 2, 2);

    this->setLayout(layout);
}