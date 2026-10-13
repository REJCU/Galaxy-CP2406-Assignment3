#include "SimulationConfig.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <system_error>

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
    // TODO - see if i need &idx
    int validValue = std::stoi(text);
    return validValue;
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::out_of_range("out of range Value for " + optionName + ": " +
                            text);

    // find out if they want mixed input allowed - if 10e or e10 is allowed
  }
}

int convertToDouble(const std::string &text, const std::string &optionName) {
  try {
    double validValue = std::stod(text);
    return validValue;
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::out_of_range("out of range Value for " + optionName + ": " +
                            text);
  }
}

int convertToLongInt(const std::string &text, const std::string &optionName) {
  try {
    long validValue = std::stoll(text);
    return validValue;
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::out_of_range("out of range Value for " + optionName + ": " +
                            text);
  }
}

int convertToInt32(const std::string &text, const std::string &optionName) {
  try {
    int32_t validValue = std::stoul(text);
    return validValue;
  } catch (const std::invalid_argument &) {
    throw std::invalid_argument("invalid value for " + optionName + ": " +
                                text);
  } catch (const std::out_of_range &) {
    throw std::out_of_range("out of range Value for " + optionName + ": " +
                            text);
  }
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

    if (argument == "--bodies") {
      const int value = convertToInt(readValue(), argument);
      if (value < 0) {
        throw std::invalid_argument("--bodies cannot be negative");
      }
      config.bodyCount = value;
    } else if (argument == "--system-size") {
      config.systemSizeAU = std::stod(readValue());
    } else if (argument == "--steps") {
      const int value = convertToInt(readValue(), argument);
      if (value < 0) {
        throw std::invalid_argument("--steps cannot be negative");
      }
      config.steps = value;
    } else if (argument == "--dt") {
      const double value = convertToDouble(readValue(), argument);
      if (value < 0) {
        throw std::invalid_argument("--delta time cannot be negative");
      }
      config.dtSeconds = value;
    } else if (argument == "--collision-threshold") {
      const double value = convertToDouble(readValue(), argument);
      if (value < 0) {
        throw std::invalid_argument("--collision-threshold cannot be negative");
      }
      config.collisionThresholdAU = value;
    } else if (argument == "--collision-samples") {
      // TODO - need to test long int and double
      const long long value = convertToLongInt(readValue(), argument);
      if (value < 0) {
        throw std::invalid_argument("--collision-samples cannot be negative");
      }
      config.collisionSamples = value;
    } else if (argument == "--seed") {
      const long long value = convertToLongInt(readValue(), argument);
      if (value < 0 || value > std::numeric_limits<std::uint32_t>::max()) {
        throw std::invalid_argument("--seed must be between 0 and 4294967295");
      }
      config.seed = static_cast<std::uint32_t>(value);
    } else if (argument == "--output") {
      config.outputCsv = readValue();
    } else if (argument == "--help") {
      std::cout << helpText() << std::endl;
      exit(0);
      //      throw std::runtime_error(helpText());
      // need to exit cleanly
    } else {
      throw std::invalid_argument("unknown option: " + argument);
    }
  }

  config.validate();
  return config;
}
