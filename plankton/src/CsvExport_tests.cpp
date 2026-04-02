#include "CsvExport.h"
#include "SimulationRecord.h"
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(CsvExport, WriteCsvWritesHeaderLine) {
    // Arrange
    const std::vector<SimulationRecord> records={
        {0.05, 5.0, 185.3},
        {0.06, 4.9, 182.1}
    };
    const double dt = 0.01;
    std::ostringstream os;
    const std::string expectedTitle("t (days), X (μg C/mL), S (mg N/L), I_avg (μmol/m²/s)");
    std::string actualTitle;

    // Act
    writeCsv( os, records, dt);
    const auto csv = os.str();
    std::istringstream iss(csv);
    std::getline(iss, actualTitle);

    // Assert
    EXPECT_EQ(actualTitle, expectedTitle);
}

TEST(CsvExport, WriteCsvWritesRecords) {
    // Arrange
    const std::vector<SimulationRecord> records={
        {0.05, 5.0, 185.3},
        {0.06127, 4.9, 182.1}
    };
    const double dt = 0.01;
    std::ostringstream os;

    // Act
    writeCsv( os, records, dt);
    const auto csv = os.str();
    std::istringstream iss(csv);
    std::string line;
    std::getline(iss, line); // ignore the title
    std::vector<std::string> lines;
    while (std::getline(iss, line)) {
        lines.emplace_back(line);
    }

    // Assert
    ASSERT_EQ(lines.size(), records.size());
    EXPECT_EQ(lines[0], "0.00, 0.0500, 5.0000, 185.3000");
    EXPECT_EQ(lines[1], "0.01, 0.0613, 4.9000, 182.1000");
}
