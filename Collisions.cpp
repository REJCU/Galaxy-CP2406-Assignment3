#include "Collisions.h"
#include "Types.h"
#include <random>
#include <stdexcept>
#include <algorithm>

bool checkAndMergeCollision(body &a, body &b, double thresholdAU) {
  if (thresholdAU < 0) {
    throw std::invalid_argument("collision threshold cannot be negative");
  }
  if (thresholdAU == 0) {
    return false;
  }
  if (&a == &b) {
    return false;
  }
  if (a.mass <= 0.0 || b.mass <= 0.0) {
    return false;
  }
  { // euclidean distance
    const double dx = a.position.x - b.position.x;
    const double dy = a.position.y - b.position.y;
    const double dz = a.position.z - b.position.z;
    const double distanceSquared = dx * dx + dy * dy + dz * dz;

    if (distanceSquared > thresholdAU * thresholdAU) {
      return false;
    }
  }
  const double totalMass = a.mass + b.mass;
  if (totalMass <= 0) {
    throw std::invalid_argument("invalid merge");
  }
  vec3 mergedPosition = (a.position * a.mass + b.position * b.mass) / totalMass;
  vec3 mergedVelocity = (a.velocity * a.mass + b.velocity * b.mass) / totalMass;

  a.mass = totalMass;
  a.position = mergedPosition;
  a.velocity = mergedVelocity;

  b.mass = 0.0;
  b.velocity = vec3{0, 0, 0};
  return true;
}

CollisionStats sampleAndMergeCollisions(std::vector<body> &bodies,
                                        double thresholdAU,
                                        std::size_t attempts,
                                        std::uint32_t seed) {
  if (thresholdAU < 0) {
    throw std::invalid_argument("collision threshold cannot be negative");
  }

  CollisionStats stats{};

  if (bodies.empty()) {
    return stats;
      }

  if (bodies.size() >= 2) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<std::size_t> pick(0, bodies.size() - 1);

    for (std::size_t attempt = 0; attempt < attempts; ++attempt) {
      ++stats.attemptedPairs;

      const std::size_t indexA = pick(generator);
      const std::size_t indexB = pick(generator);

      if (indexA == indexB) {
        continue;
      }
      if (bodies[indexA].mass <= 0.0 || bodies[indexB].mass <= 0.0) {
        continue;
      }

      ++stats.validPairs;
      if (checkAndMergeCollision(bodies[indexA], bodies[indexB], thresholdAU))
        ++stats.mergedPairs;
    }
  }

stats.activeBodies = static_cast<std::size_t>(
	std::count_if(bodies.begin(), bodies.end(), [](const body& value)
    {return value.mass;  }));
 return stats;
}
  

