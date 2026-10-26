#include "Collisions.h"
#include "Simulation.h"
#include "Types.h"
#include <iostream>
#include <random>
#include <stdexcept>

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

  CollisionStats statistics;
  statistics.attemptedPairs = attempts;

  if (bodies.size() >= 2) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<std::size_t> pick(0, bodies.size() - 1);

    for (std::size_t attempt = 0; attempt < attempts; ++attempt) {
      ++statistics.attemptedPairs;

      const std::size_t indexA = pick(generator);
      const std::size_t indexB = pick(generator);

      if (indexA == indexB) {
        continue;
      }
      if (bodies[indexA].mass <= 0.0 || bodies[indexB].mass <= 0.0) {
        continue;
      }

      ++statistics.validPairs;
      if (checkAndMergeCollision(bodies[indexA], bodies[indexB], thresholdAU))
        ++statistics.mergedPairs;
    }
  }
  for (const body &currentBody : bodies) {
    if (currentBody.mass > 0) {
      ++statistics.activeBodies;
    }
  }
  return statistics;
}
