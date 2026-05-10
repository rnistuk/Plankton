#pragma once
#include "SimulationRecord.h"

#include <QGroupBox>
#include <QChart>
#include <QChartView>

class Results : public QGroupBox {
    Q_OBJECT
public:
    Results(QWidget* parent = nullptr);
    void setRecords(double dt, const std::vector<SimulationRecord>& records);

private:
    QChart* chart;
    QChartView* chartView;

    QChart* createChart();
};
