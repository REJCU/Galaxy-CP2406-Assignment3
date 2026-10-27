#include "CsvReporter.h"
#include "Collisions.h"

#include <fstream>
#include <iomanip>

void writeExperimentCsv(const std::string &file, const std::string &name,
                        const SimulationConfig &, const ExperimentResult &) {
  std::ifstream output(
      file); // was ofstream - have to see why and if changing is needed
  bool fileExists = output.good();
  output.close();

  output << "experiment,status\n" << name << ",baseline\n";
}
