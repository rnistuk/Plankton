#pragma once
#include "SimulationRecord.h"

#include <QChartView>
#include <QGroupBox>
#include <QLineSeries>

class Results : public QGroupBox {
    Q_OBJECT
public:
    Results(QWidget* parent = nullptr);
    void setRecords(double dt, const std::vector<SimulationRecord>& records);

private:
    QChart* chart;
    QChartView* chartView;
    QLineSeries* xSeries;
    QLineSeries* sSeries;
};
