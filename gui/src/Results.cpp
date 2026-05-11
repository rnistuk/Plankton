#include "Results.h"

#include <iostream>
#include <QBoxLayout>
#include <QLineSeries>

Results::Results(QWidget* parent) : QGroupBox(parent) {
    this->setTitle("Results");
    this->chartView = new QChartView(this);
    this->chartView->setObjectName("Results Chart View");

    this->chart = this->createChart();
    this->chart->setObjectName("Results Chart");


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

    return chart;
}

void Results::setRecords(double dt, const std::vector<SimulationRecord>& records) {
    this->chart->removeAllSeries();
    QLineSeries* xSeries = new QLineSeries;
    xSeries->setName("Biomass X (g/L)");

    QLineSeries* sSeries = new QLineSeries;
    sSeries->setName("Substrate S (g/L)");
    double t = 0.0;
    for (const auto& record : records) {
        xSeries->append(t, record.X);
        sSeries->append(t, record.S);
        t += dt;
    }

    this->chart->addSeries(xSeries);
    this->chart->addSeries(sSeries);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Time (days)");
    chart->axes(Qt::Vertical).first()->setTitleText("Concentration (g/L)");
}
