#pragma once

#include <QGroupBox>
#include <QChart>
#include <QChartView>

class Results : public QGroupBox {
    Q_OBJECT
public:
    Results(QWidget* parent = nullptr);

private:
    QChart* chart;
    QChartView* chartView;

    QChart* createChart();
};
