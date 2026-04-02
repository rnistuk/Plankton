#pragma once
#include <ostream>
#include <vector>

class SimulationRecord;

void writeCsv(std::ostream&, const std::vector<SimulationRecord>&, double dt);
