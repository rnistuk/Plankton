#include "CsvExport.h"
#include "SimulationRecord.h"
#include <iomanip>
#include <ostream>

namespace {
    void writeRecord(std::ostream& os, double time_days, const SimulationRecord& record) {
        os << std::fixed << std::setprecision(2) << time_days << ", "
            << std::setprecision(4) << record.X << ", " << record.S
            << ", " << record.I_avg << "\n";
    }
}

void writeCsv(std::ostream& os, const std::vector<SimulationRecord>& records, double dt) {
    const std::string title("t (days), X (μg C/mL), S (mg N/L), I_avg (μmol/m²/s)");
    os << title << "\n";
    size_t i = 0;
    for (const auto& record : records) {
        writeRecord(os, static_cast<double>(i++) * dt, record);
    }
}
