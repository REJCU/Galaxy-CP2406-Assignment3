#include "SimulationConfig.h"

#include <numeric>
#include <stdexcept>
#include <string>

// TODO - need to test the new inputs and record the results
std::string helpText() {
  return "Options: --bodies N --system-size AU --steps N "
         "--dt SECONDS --collision-threshold AU "
         "--collision-samples N --seed N --output FILE --help";
}
void SimulationConfig::validate() const {
  if (bodyCount < 2) {
    throw std::invalid_argument("body count must be at least 2");
  }

  if (systemSizeAU <= 0.0) {
    throw std::invalid_argument("system size must be positive");
  }

  if (steps < 1) {
    throw std::invalid_argument("steps must be at least 1");
  }

  if (dtSeconds <= 0) {
    throw std::invalid_argument("time step must be positive");
  }

  if (collisionThresholdAU < 0.0) {
    throw std::invalid_argument("collision threshold cannot be negative. Input "
                                "zero to disable collisions");
    // threshold of zero disables merging
  }
}

// helper functions to safely handle missing values, text that is not a number,
// extra characters, negative numbers, large int numbers
int convertToInt(const std::string &text, const std::string &optionName) {
  try {
    int x = std::stoi(text);
    return x;
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  }
}

// float convertToFloat(const std::string string &text,
//                      const std::string &optionName) {}

SimulationConfig parseArguments(int argc, char **argv) {
  SimulationConfig config;

  for (int argumentIndex = 1; argumentIndex < argc; ++argumentIndex) {
    const std::string argument = argv[argumentIndex];
    if (argument == "--help") {
      helpText();
    }

    const auto readValue = [&]() {
      if (argumentIndex + 1 >= argc) {
        throw std::invalid_argument("missing value for " + argument);
      }
      return std::string(argv[++argumentIndex]);
    };

    if (argument == "--bodies") {
      config.bodyCount = convertToInt(readValue(), argument);
    } else if (argument == "--system-size") {
      config.systemSizeAU = std::stod(readValue());
    } else if (argument == "--steps") {
      config.steps = std::stoi(readValue());
    } else if (argument == "--dt") {
      config.dtSeconds = std::stod(readValue());
    } else if (argument == "--collision-threshold") {
      config.collisionThresholdAU = std::stod(readValue());
    } else if (argument == "--collision-samples") {
      config.collisionSamples = std::stoull(readValue());
    } else if (argument == "--seed") {
      config.seed = static_cast<std::uint32_t>(std::stoul(readValue()));
    } else if (argument == "--output") {
      config.outputCsv = readValue();
    } else if (argument == "--help") {
      throw std::runtime_error(helpText());
    } else {
      throw std::invalid_argument("unknown option: " + argument);
    }
  }

  config.validate();
  return config;
}
