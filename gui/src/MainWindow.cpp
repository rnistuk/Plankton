#include "MainWindow.h"
#include "InitialConditions.h"
#include "KineticParameters.h"
#include "ReactorGeometry.h"
#include "Results.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)  : QMainWindow(parent) {
    this->root = new QWidget(this);

    auto* initial = new InitialConditions();
    auto* reactor = new ReactorGeometry();
    auto* kinetic = new KineticParameters();
    auto* results = new Results();


    auto* inputLayout = new QVBoxLayout();
    inputLayout->addWidget(initial);
    inputLayout->addWidget(reactor);
    inputLayout->addWidget(kinetic);

    auto* resultLayout = new QVBoxLayout();
    resultLayout->addWidget(results);

    auto* layout = new QHBoxLayout();
    layout->addLayout(inputLayout);
    layout->addLayout(resultLayout);

    root->setLayout(layout);
    setCentralWidget(root);
}

MainWindow::~MainWindow() {
}
