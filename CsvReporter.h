#ifndef CSV_REPORTER_H
#define CSV_REPORTER_H

#include "Simulation.h"

#include <string>
#include <vector>

struct ExperimentRecord {
  std::string name;
  std::string status;
  SimulationConfig config;
  ExperimentResult result;
};

void writeExperimentCsv(const std::string &file,
                        const std::vector<ExperimentRecord> &records);

#endif // CSV_REPORTER_H
