#include "Results.h"

#include <algorithm>
#include <QBoxLayout>
#include <QLineSeries>

Results::Results(QWidget* parent) : QGroupBox(parent) {
    this->setTitle("Results");
    this->chartView = new QChartView(this);
    this->chartView->setObjectName("Results Chart View");

    this->chart = new QChart();
    this->chart->setTitle("Plankton Simulation");
    this->chart->setObjectName("Results Chart");
    this->chartView->setChart(this->chart);

    this->xSeries = new QLineSeries;
    this->xSeries->setObjectName("X");
    this->xSeries->setName("Biomass X (g/L)");

    this->sSeries = new QLineSeries;
    this->sSeries->setObjectName("S");
    this->sSeries->setName("Substrate S (g/L)");

    chart->addSeries(this->xSeries);
    chart->addSeries(this->sSeries);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Time (days)");
    chart->axes(Qt::Vertical).first()->setTitleText("Concentration (g/L)");

    auto* layout = new QVBoxLayout;
    layout->addWidget(this->chartView);
    this->setLayout(layout);
}

void Results::setRecords(double dt, const std::vector<SimulationRecord>& records) {
    this->xSeries->clear();
    this->sSeries->clear();

    double maxConc = 0.0;
    for (size_t i = 0; i < records.size(); ++i) {
        const double t = i * dt;
        this->xSeries->append(t, records[i].X);
        this->sSeries->append(t, records[i].S);
        maxConc = std::max(maxConc, std::max(records[i].X, records[i].S));
    }

    chart->axes(Qt::Horizontal).first()->setRange(0, records.size() * dt);
    chart->axes(Qt::Vertical).first()->setRange(0, maxConc * 1.01);
}
