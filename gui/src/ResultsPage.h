#pragma once
#include "PlanktonWizardPage.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QCheckBox>
#include <QPushButton>
#include "SimulationRecord.h"

class ResultsPage : public PlanktonWizardPage {
    Q_OBJECT
public:
    explicit ResultsPage(QWidget *parent = nullptr);
    void initializePage() override;
    bool validateInputs() override { return true; }
    void populateFromFields() override {}

private slots:
    void updateSeriesVisibility();
    void exportCsv();

private:
    QChartView *m_chartView;
    QLineSeries *m_xSeries;
    QLineSeries *m_sSeries;
    // Removed m_iSeries

    QCheckBox *m_xCheck;
    QCheckBox *m_sCheck;
    // Removed m_iCheck
    QPushButton *m_exportBtn;

    std::vector<SimulationRecord> m_results;
    double m_dt = 0.01;
};