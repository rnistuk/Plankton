#pragma once
#include <ostream>
#include <vector>

struct SimulationRecord;

void writeCsv(std::ostream&, const std::vector<SimulationRecord>&, double dt);
