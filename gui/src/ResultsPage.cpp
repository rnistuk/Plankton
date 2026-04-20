#include "ResultsPage.h"
#include "Simulation.h"
#include "BeerLambert.h"
#include "CsvExport.h"
#include "ConfigAssembler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>

ResultsPage::ResultsPage(QWidget *parent) : PlanktonWizardPage(parent) {
    setTitle("Simulation Results");

    m_chartView = new QChartView;
    m_chartView->setRenderHint(QPainter::Antialiasing);

    auto *chart = new QChart;
    m_chartView->setChart(chart);

    m_xSeries = new QLineSeries; m_xSeries->setName("Biomass X (g/L)");
    m_sSeries = new QLineSeries; m_sSeries->setName("Substrate S (g/L)");

    chart->addSeries(m_xSeries);
    chart->addSeries(m_sSeries);

    m_xCheck = new QCheckBox("Biomass X"); m_xCheck->setChecked(true);
    m_sCheck = new QCheckBox("Substrate S"); m_sCheck->setChecked(true);
    m_exportBtn = new QPushButton("Export CSV");

    connect(m_xCheck, &QCheckBox::toggled, this, &ResultsPage::updateSeriesVisibility);
    connect(m_sCheck, &QCheckBox::toggled, this, &ResultsPage::updateSeriesVisibility);
    connect(m_exportBtn, &QPushButton::clicked, this, &ResultsPage::exportCsv);

    auto *controls = new QHBoxLayout;
    controls->addWidget(m_xCheck);
    controls->addWidget(m_sCheck);
    controls->addStretch();
    controls->addWidget(m_exportBtn);

    auto *layout = new QVBoxLayout;
    layout->addWidget(m_chartView);
    layout->addLayout(controls);
    setLayout(layout);
}

void ResultsPage::initializePage() {
    using namespace plankton::gui;
    ConfigAssembler assembler;
    try {
        // Fix 1: Initialize config directly to avoid the deleted default constructor error
        const RunConfig config = assembler.assemble(wizard());
        m_dt = config.params.dt;

        // Fix 2: Define the light model lambda and run simulation inside the try block
        // This ensures 'config' is in scope and valid when used.
        const auto lightModel = [&config](double X) {
            return depthAveragedIrradiance(config.params.reactor, X);
        };

        m_results = simulate(static_cast<size_t>(config.numSteps),
                            config.initialState,
                            config.params,
                            lightModel);

        // 3. Update chart series
        m_xSeries->clear();
        m_sSeries->clear();

        double max_X = m_results.front().X;
        double max_S = m_results.front().X;

        for (int i = 0; i < m_results.size(); ++i) {
            double t = i * m_dt;
            max_X = std::max(max_X, m_results[i].X);
            max_S = std::max(max_S, m_results[i].S);

            m_xSeries->append(t, m_results[i].X);
            m_sSeries->append(t, m_results[i].S);
        }

        double max_Vertical = std::max(max_X, max_S) * 1.01;

        // 4. Update Axes and fix the "1-day" bug
        auto *chart = m_chartView->chart();
        chart->createDefaultAxes();

        // Explicitly set the horizontal axis range to the total duration
        if (!chart->axes(Qt::Horizontal).isEmpty()) {
            auto *axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
            if (axisX) {
                double totalDuration = (static_cast<double>(m_results.size()) - 1.0) * m_dt;
                axisX->setRange(0, totalDuration);
                axisX->setTitleText("Time (days)");
            }

            auto* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
            if (axisY) {
                axisY->setRange(0,max_Vertical);
            }




        }

        if (!chart->axes(Qt::Vertical).isEmpty()) {
            chart->axes(Qt::Vertical).first()->setTitleText("Value");
        }

        chart->setTitle("Plankton Growth Dynamics");
        updateSeriesVisibility();

    } catch (const std::exception& e) {
        // Handle potential domain logic errors (e.g., negative values from assembler)
        QMessageBox::critical(this, "Configuration Error", e.what());
    }




}






void ResultsPage::updateSeriesVisibility() {
    m_xSeries->setVisible(m_xCheck->isChecked());
    m_sSeries->setVisible(m_sCheck->isChecked());
}

void ResultsPage::exportCsv() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export CSV", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) return;

    std::ofstream file(fileName.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
        return;
    }

    writeCsv(file, m_results, m_dt);
    QMessageBox::information(this, "Success", "Simulation results exported successfully.");
}
