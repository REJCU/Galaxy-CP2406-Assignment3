#include "SimulationConfig.h"
#include "Simulation.h"

#include <cmath>
#include <limits>
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
  std::size_t pos = 0;
  int value = 0;
  try {
    value = std::stoi(text, &pos);
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::out_of_range("out of range value for " + optionName + ": " +
                            text);
  }
  if (pos != text.size()) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  }
  return value;
}

double convertToDouble(const std::string &text, const std::string &optionName) {
  std::size_t pos = 0;
  double value = 0.0;
  try {
    value = std::stod(text, &pos);
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::out_of_range("out of range value for " + optionName + ": " +
                            text);
  }
  if (pos != text.size() || !std::isfinite(value)) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  }
  return value;
}

long long convertToLongLong(const std::string &text,
                            const std::string &optionName) {
  std::size_t pos = 0;
  long long value = 0;
  try {
    value = std::stoll(text, &pos);
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::out_of_range("out of range value for " + optionName + ": " +
                            text);
  }
  if (pos != text.size()) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  }
  return value;
}

SimulationConfig parseArguments(int argc, char **argv) {
  SimulationConfig config;

  for (int argumentIndex = 1; argumentIndex < argc; ++argumentIndex) {
    const std::string argument = argv[argumentIndex];

    const auto readValue = [&]() {
      if (argumentIndex + 1 >= argc) {
        throw std::invalid_argument("missing value for " + argument);
      }
      return std::string(argv[++argumentIndex]);
    };
    if (argument == "--help") {
      throw helpText();
    } else if (argument == "--bodies") {
      config.bodyCount = convertToInt(readValue(), argument);
    } else if (argument == "--system-size") {
      config.systemSizeAU = convertToDouble(readValue(), argument);
    } else if (argument == "--steps") {
      config.steps = convertToInt(readValue(), argument);
    } else if (argument == "--dt") {
      config.dtSeconds = convertToDouble(readValue(), argument);
    } else if (argument == "--collision-threshold") {
      config.collisionThresholdAU = convertToDouble(readValue(), argument);
    } else if (argument == "--collision-samples") {
      const long long value = convertToLongLong(readValue(), argument);
      if (value < 0) {
        throw std::invalid_argument("--collision-samples cannot be negative");
      }
      config.collisionSamples =
          static_cast<decltype(config.collisionSamples)>(value);
    } else if (argument == "--seed") {
      const long long value = convertToLongLong(readValue(), argument);
      if (value < 0 || value > std::numeric_limits<std::uint32_t>::max()) {
        throw std::invalid_argument("--seed must be between 0 and 4294967295");
      }
      config.seed = static_cast<std::uint32_t>(value);

    } else if (argument == "--output") {
      config.outputCsv = readValue();
    } else {
      throw std::invalid_argument("unknown option: " + argument);
    }
  }

  config.validate();
  return config;
}
