#include "Collisions.h"
#include "SimulationConfig.h"
#include "Types.h"

#include <iostream>
#include <stdexcept>
#include <vector>

body makeBody(double mass, double x, double vx) {
  body b{};
  b.mass = mass;
  b.position.x = x;
  b.velocity.x = vx;
  return b;
}

void check(const char *name, bool ok) {
  if (ok) {
    std::cout << "pass " << name << "\n";
  } else {
    std::cout << "fail " << name << "\n";
  }
}

int main() {
  // Test 1 - dt of 0 should be rejected
  SimulationConfig config;
  config.dtSeconds = 0;
  bool threw = false;
  try {
    config.validate();
  } catch (const std::invalid_argument &e) {
    threw = true;
    std::cout << "message: " << e.what() << "\n";
  }
  check("zero dt rejected", threw);

  // Test 2 - zero threshold should not merge, even at the same position
  body a = makeBody(1, 0, 0);
  body b = makeBody(1, 0, 0);
  check("zero threshold no merge", checkAndMergeCollision(a, b, 0) == false);

  // Test 3 - distance exactly equal to threshold merges
  // by hand: mass 3, x = 1, velocity = 0
  body c = makeBody(2, 0, 1);
  body d = makeBody(1, 3, -2);
  bool merged = checkAndMergeCollision(c, d, 3);
  check("exact threshold merge", merged && c.mass == 3 && c.position.x == 1 &&
                                     c.velocity.x == 0 && d.mass == 0);

  // Test 4 - retired body is ignored
  body e = makeBody(1, 0, 0);
  body retired = makeBody(0, 0.1, 0);
  check("retired body ignored", checkAndMergeCollision(e, retired, 1) == false);

  // Test 5 - same seed gives same result, and attempts are limited to 10
  std::vector<body> first = {makeBody(2, 0, 1), makeBody(1, 0.5, -2),
                             makeBody(0, 0.2, 0)};
  std::vector<body> second = first;

  CollisionStats s1 = sampleAndMergeCollisions(first, 1.0, 10, 42);
  CollisionStats s2 = sampleAndMergeCollisions(second, 1.0, 10, 42);

  std::cout << "run 1: attempted " << s1.attemptedPairs << " valid "
            << s1.validPairs << " merged " << s1.mergedPairs << " active "
            << s1.activeBodies << "\n";

  std::cout << "run 2: attempted " << s2.attemptedPairs << " valid "
            << s2.validPairs << " merged " << s2.mergedPairs << " active "
            << s2.activeBodies << "\n";

  check("attempts limited to 10", s1.attemptedPairs == 10);
  check("same seed same result", s1.validPairs == s2.validPairs &&
                                     s1.mergedPairs == s2.mergedPairs &&
                                     s1.activeBodies == s2.activeBodies);

  return 0;
}
