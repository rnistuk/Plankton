#include "Results.h"
#include "SimulationRecord.h"

#include <QLineSeries>
#include <gtest/gtest.h>

void children(QObject *parent, std::vector<QObject *> &c) {
    for (QObject *child: parent->children()) {
        c.push_back(child);
        children(child, c);
    }
}

TEST(ResultsTest, SetRecordsPopulatesSeries) {
    // Arrange
    double dt = 0.01;
    Results r(nullptr);
    std::vector<SimulationRecord> records = {{1.0, 2.0, 50.0}, {1.1, 1.9, 50.0}};

    // Act
    r.setRecords(dt, records);
    auto *cv = r.findChild<QChartView *>("Results Chart View");
    ASSERT_NE(cv, nullptr);

    auto *cn = cv->chart();
    ASSERT_NE(cn, nullptr);

    EXPECT_EQ(cn->series().size(), 2);
    QLineSeries* xs = qobject_cast<QLineSeries *>(cn->series()[0]);
    QLineSeries* ss = qobject_cast<QLineSeries *>(cn->series()[1]);

    ASSERT_NE(xs, nullptr);
    ASSERT_NE(ss, nullptr);

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
