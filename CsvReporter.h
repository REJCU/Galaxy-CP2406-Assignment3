#ifndef CSV_REPORTER_H
#define CSV_REPORTER_H

#include "Simulation.h"
#include "SimulationConfig.h"

#include <string>

struct ExperimentRecord {
  std::string name;
  std::string status;
  SimulationConfig config;
  ExperimentResult result;
}; // can safely delete

void writeExperimentCsv(const std::string &file, const std::string &name,
                        const SimulationConfig &config,
                        const ExperimentResult &result);

#endif // CSV_REPORTER_H
