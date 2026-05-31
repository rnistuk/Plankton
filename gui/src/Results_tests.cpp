#include "Results.h"
#include "SimulationRecord.h"

#include <gtest/gtest.h>
#include <QLineSeries>

constexpr char ResultsChartViewName[] = "Results Chart View";

void assertSeriesMatchesRecords(QLineSeries* xs
    , QLineSeries* ss
    , double dt
    , const std::vector<SimulationRecord> &records) {
    int i = 0;
    double t = 0.0;

    ASSERT_EQ(xs->points().size(), records.size());
    ASSERT_EQ(ss->points().size(), records.size());

    for (const auto& r : records) {
        ASSERT_DOUBLE_EQ(t, xs->points()[i].x());
        ASSERT_DOUBLE_EQ(t, ss->points()[i].x());
        ASSERT_DOUBLE_EQ(r.X, xs->points()[i].y());
        ASSERT_DOUBLE_EQ(r.S, ss->points()[i].y());
        ++i;
        t = i * dt;
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
    auto series = findSeriesInChartViewNamed(ResultsChartViewName, r);

    // Assert
    ASSERT_FALSE(series.empty());
    EXPECT_EQ(series.size(), 2);
    QLineSeries* xs = qobject_cast<QLineSeries *>(series[0]);
    QLineSeries* ss = qobject_cast<QLineSeries *>(series[1]);
    ASSERT_NE(xs, nullptr);
    ASSERT_NE(ss, nullptr);
    assertSeriesMatchesRecords(xs, ss, dt, records);
}

TEST(ResultsTest, SetRecordsKeepsSeriesPointers) {
    // Arrange
    constexpr double dt = 0.01;
    const std::vector<SimulationRecord> records = {{1.0, 2.0, 50.0}, {1.1, 1.9, 50.0}};
    const std::vector<SimulationRecord> newRecords = {{2.0, 3.0, 60.0}, {2.1, 3.9, 61.0}};

    // Act
    Results r(nullptr);
    r.setRecords(dt, records);

    // Assert
    auto series = findSeriesInChartViewNamed(ResultsChartViewName, r);
    EXPECT_EQ(series.size(), 2);
    auto *xs = qobject_cast<QLineSeries *>(series[0]);
    auto *ss = qobject_cast<QLineSeries *>(series[1]);
    ASSERT_NE(xs, nullptr);
    ASSERT_NE(ss, nullptr);

    ASSERT_EQ(xs->objectName().toStdString(), "X");
    ASSERT_EQ(ss->objectName().toStdString(), "S");

    r.setRecords(dt, newRecords);
    assertSeriesMatchesRecords(xs, ss, dt, newRecords);
}

TEST(ResultsTest, SetRecordsUpdatesEmptySeriesDataByClearing) {
    // Arrange
    constexpr double dt = 0.01;
    Results r(nullptr);
    std::vector<SimulationRecord> records = {{1.0, 2.0, 50.0}, {1.1, 1.9, 50.0}};
    r.setRecords(dt, records);

    // Act
    r.setRecords(dt, {});

    // Assert
    auto series = findSeriesInChartViewNamed(ResultsChartViewName, r);
    ASSERT_FALSE(series.empty());
    EXPECT_EQ(series.size(), 2);
    QLineSeries* xs = qobject_cast<QLineSeries *>(series[0]);
    QLineSeries* ss = qobject_cast<QLineSeries *>(series[1]);
    ASSERT_NE(xs, nullptr);
    ASSERT_NE(ss, nullptr);
    ASSERT_TRUE(xs->points().isEmpty());
    ASSERT_TRUE(ss->points().isEmpty());
}
