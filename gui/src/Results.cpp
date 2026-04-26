#include "Results.h"

#include <QBoxLayout>
#include <QLineSeries>

Results::Results(QWidget* parent) : QGroupBox(parent) {
    this->setTitle("Results");
    this->chartView = new QChartView(this);

    this->chart = this->createChart();
    this->chartView->setChart(this->chart);

    QLineSeries* xSeries = new QLineSeries;
    xSeries->setName("Biomass X (g/L)");

    QLineSeries* sSeries = new QLineSeries;
    sSeries->setName("Substrate S (g/L)");

    chart->addSeries(xSeries);
    chart->addSeries(sSeries);


    auto* layout = new QVBoxLayout;

    layout->addWidget(this->chartView);

    this->setLayout(layout);
}

QChart* Results::createChart() {
    QChart* chart = new QChart();

    chart->setTitle("Plankton Simulation");
    chart->createDefaultAxes();

    return chart;
}
