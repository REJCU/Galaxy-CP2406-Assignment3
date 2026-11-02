#include "CsvReporter.h"
#include "Simulation.h"
#include "SimulationConfig.h"
#include "Types.h"
#include "Renderer.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>


//rendering - clearoldframes

//renderer - createvideo
// rendering was the main culprit


// namespace

struct Run {
  std::string name;
  std::size_t bodies;
  double threshold;
  int deltaTime;
};

void runExperiment(const SimulationConfig &baseline) {
  std::vector<Run> runs = {
      {"A-disabled", 1024, 0, 1024000},
      {"A-rare", 1024, 0.1, 1024000},
      {"A-higher", 1024, 1.0, 1024000},
      {"A-higher-dt", 1024, 1.0, 2048000},
      {"B-disabled", 2048, 0, 1024000},
      {"B-rare", 2048, 0.1, 1024000},
      {"B-higher", 2048, 1.0, 1024000},
      {"A-rare-repeat", 1024, 0.1, 1024000},
      {"A-higher-repeat", 1024, 1.0, 1024000},
  };

  std::sort(runs.begin(), runs.end(),
            [](const Run &x, const Run &y) { return x.name < y.name; });

  for (const Run &run : runs) {
    SimulationConfig config = baseline;
    config.bodyCount = run.bodies;
    config.collisionThresholdAU = run.threshold;
    config.dtSeconds = run.deltaTime;

    std::vector<body> bodies = initialiseBodies(config);
    const ExperimentResult result = runExperiment(bodies, config, "images");
    writeExperimentCsv(config.outputCsv, run.name, config, result);
    std::cout << "Finished " << run.name << '\n';
  }
}

int main(int argc, char **argv) {
  try {
    const SimulationConfig config = parseArguments(argc, argv);
    clearOldFrames();
    std::filesystem::remove(config.outputCsv);
    
    runExperiment(config);

    std::cout << "Created " << config.steps << " frames in images/.\n";

    if (createVideo()) {
      std::cout << "Created galaxy.mp4\n";
    } else {
      std::cerr << "Frames were created, but FFmpeg was not found or failed.\n";
    }

    std::cout << "Completed baseline run\n";
    return 0;
  } catch (const std::runtime_error &error) {
    std::cout << error.what() << '\n';
    return 0;
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
}
