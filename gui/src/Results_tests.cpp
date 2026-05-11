#include "Results.h"
#include "SimulationRecord.h"

#include <gtest/gtest.h>
#include <QLineSeries>

void children(QObject *parent, std::vector<QObject *> &c) {
    for (QObject *child: parent->children()) {
        c.push_back(child);
        children(child, c);
    }
}

QList<QAbstractSeries *> findSeriesInChartViewNamed(const QString &name, Results &r) {
    auto *cv = r.findChild<QChartView *>(name);
    if (cv == nullptr) {
        return {};
    }
    auto *cn = cv->chart();
    if (cn == nullptr) {
        return {};
    }
    return cn->series();
}

TEST(ResultsTest, SetRecordsPopulatesSeries) {
    // Arrange
    constexpr double dt = 0.01;
    Results r(nullptr);
    std::vector<SimulationRecord> records = {{1.0, 2.0, 50.0}, {1.1, 1.9, 50.0}};

    // Act
    r.setRecords(dt, records);
    auto *cv = r.findChild<QChartView *>("Results Chart View");
    ASSERT_NE(cv, nullptr);

    auto series = findSeriesInChartViewNamed("Results Chart View", r);

    ASSERT_FALSE(series.empty());
    EXPECT_EQ(series.size(), 2);
    QLineSeries* xs = qobject_cast<QLineSeries *>(series[0]);
    QLineSeries* ss = qobject_cast<QLineSeries *>(series[1]);

    int i = 0;
    double t = 0.0;
    for (const auto& r : records) {
        ASSERT_DOUBLE_EQ(t, xs->points()[i].x());
        ASSERT_DOUBLE_EQ(t, ss->points()[i].x());
        ASSERT_FLOAT_EQ(r.X, xs->points()[i].y());
        ASSERT_FLOAT_EQ(r.S, ss->points()[i].y());
        ++i;
        t += dt;
    }
}

TEST(ResultsTest, SetRecordsKeepsSeriesPointers) {
    // Arrange
    constexpr double dt = 0.01;
    const std::vector<SimulationRecord> records = {{1.0, 2.0, 50.0}, {1.1, 1.9, 50.0}};
    const std::vector<SimulationRecord> newRecords = {{2.0, 3.0, 60.0}, {2.1, 3.9, 61.0}};

    // Act
    Results r(nullptr);
    r.setRecords(dt, records);

    auto series = findSeriesInChartViewNamed("Results Chart View", r);
    EXPECT_EQ(series.size(), 2);
    auto *xs = qobject_cast<QLineSeries *>(series[0]);
    auto *ss = qobject_cast<QLineSeries *>(series[1]);
    ASSERT_NE(xs, nullptr);
    ASSERT_NE(ss, nullptr);

    ASSERT_EQ(xs->objectName().toStdString(), "X");
    ASSERT_EQ(ss->objectName().toStdString(), "S");

    r.setRecords(dt, newRecords);
    // Assert
    ASSERT_EQ(xs->points().size(), newRecords.size());
    ASSERT_EQ(ss->points().size(), newRecords.size());
    int i=0;
    double t = 0.0;
    for (const auto& rec : newRecords) {
        ASSERT_DOUBLE_EQ(t, xs->points()[i].x());
        ASSERT_DOUBLE_EQ(rec.X, xs->points()[i].y());
        ASSERT_DOUBLE_EQ(t, ss->points()[i].x());
        ASSERT_DOUBLE_EQ(rec.S, ss->points()[i].y());
        ++i;
        t += dt;
    }
}
