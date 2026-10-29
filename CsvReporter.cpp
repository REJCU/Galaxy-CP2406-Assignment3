#include "CsvReporter.h"
#include "Collisions.h"

#include <fstream>
#include <iomanip>

void writeExperimentCsv(const std::string &file, const std::string &name,
                        const SimulationConfig &config,
                        const ExperimentResult &result) {
  std::ifstream checkFileExists(file); // added error checking
  bool fileExists = checkFileExists.good();
  checkFileExists.close();

  std::ofstream out(file, std::ios::app);
  if (!out.is_open())
    return;

  if (!fileExists) {
    out << "experiment,status,bodyCount,systemSizeAU,steps,dtSeconds,"
           "collisionThresholdAU,collisionSamples,seed,elapsedMs,"
           "attemptedPairs,validPairs,mergedPairs,activeBodies,totalMass,"
           "barycentreX,barycentreY,barycentreZ\n";
  }

  out << std::fixed << std::setprecision(4);

  out << name << ",ok," << config.bodyCount << "," << config.systemSizeAU << ","
      << config.steps << "," << config.dtSeconds << ","
      << config.collisionThresholdAU << "," << config.collisionSamples << ","
      << config.seed << "," << result.elapsedMs << ","
      << result.collisions.attemptedPairs << "," << result.collisions.validPairs
      << "," << result.collisions.mergedPairs << "," << result.activeBodies
      << "," << result.totalMass << "," << result.barycentre.x << ","
      << result.barycentre.y << "," << result.barycentre.z << "\n";

  out.close();
};
