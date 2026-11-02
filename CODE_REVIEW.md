# Code Review

## Student and Project Details

**Student Name:** Ryan Ericson
**Student ID:** 14220594
**Project Options:** Barnes-Hut Galaxy Simulation

Answer each question from the assessment in your own words and refer to specific functions or lines in your project.

## 1. Pointer member access
**Question:** For body* a. what does a->position.x mean, and how it it related to (*a).position.x?

Responce:
a is a pointer, so it stores the memory address of the body, but not the body itself. -> follows the address and selects a member, so a->position points to the position member of the body, and `a->position.x` is the x value located inside that vec3. 

`(*a).position.x` achieves the same thing. * is dereferencing, using a pointer to access the value of the address being stored. * is used to get the value of the variable from the address. The . operator is used to select the member. 

`->` combines pointer dereferencing and member access into a single operator. 

Code Evidence: void Bhtree::singleInteract(body *target, const body *other, bool singlePart) 

`'target->accel.x += factor * rM.x;
target->accel.y += factor * rM.y;
target->accel.z += factor * rM.z;'`

Target is body*, so -> modifies the callers body. 


## 2. Octree child selection
**Question** How does Bhtree::insertIntoChild decide which child octant recieves a body? What boundary behaviour should a reviewer notice? 

Responce:
`insertintochild` is reading the body's position through the pointer 'route->position'. An if else statement that builds the child octants in turn and asks for each for one 'contains(routePos)'. The first one that returns true recieves the body. If a child octant does not exist, it is created with 

    `} else {
        Octant dse = octy.mDSE();
        if (!DSE) DSE = new Bhtree(std::move(dse));
        DSE->insert(route);`

and the body is passed down with the insert. Final else is a catchall so the last octant (DSE) recieves the body. 

    // Note: Octant::contains uses inclusive bounds, so on boundaries the first matching branch will be selected.
Inclusive - Including the last number. A body sitting exactly on a shared face/number is inside more than one child. 
A body on the boundary always goes to the earliest matching child, which is deterministic but not symmetric. 
If a body that fails all the contains, (floating point error) is acccepted into DSE silently instead of being rejected.

Code Evidence: 

    `if (Octant unw = octy.mUNW(); unw.contains(routePos)) {
        if (!UNW) UNW = new Bhtree(std::move(unw));
        UNW->insert(route);
    } else if (Octant une = octy.mUNE(); une.contains(routePos)) {

    } else {
        Octant dse = octy.mDSE();
        if (!DSE) DSE = new Bhtree(std::move(dse));
        DSE->insert(route);
    }`


## 3. Distinct collision indices
**Question** Why must a collision attempt select two distinct indices, and what can go wrong if the same body is selected twice? 
Responce:
	Collision is an interaction between two different bodies, so any attempt needs two different indices to compare. In `sampleAndMergeCollisions`, `indexA` and `indexB` are independently drawn, so they can match by chance. If the same body was passed as both a and b, the distance would be 0, which is in the positive threshold, so it would merge with itself. Body would count as retired, artificially inflating "mergedpairs". 


Code Evidence: 

`bool checkAndMergeCollision(body &a, body &b, double thresholdAU) {
  if (thresholdAU < 0) {
    throw std::invalid_argument("collision threshold cannot be negative");
  }
  if (thresholdAU == 0) {
    return false;
  }
  if (&a == &b) { // safety net for other caller
    return false;
  }
      if (indexA == indexB) { // skips the attempt before valid pair 
        continue;
	}`


## 4. Bounded Sampling  
**Question** Why must the sampling loop be bounded by attempts rather than successful collisions?
Responce: 
If the loop was to be bounded by the number of successful collisions, and collisions are rare or disabled, the loop can run indefinitely waiting for merges that might never happen. Bounding by attempts guarentees the pass always terminates and the collisionStats accuarately reports how many pairs were attempted vs valid vs merged. 


### Code Evidence: 
``for (std::size_t attempt = 0; attempt < attempts; ++attempt) {
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
``

## 5. Conservation during Merge 
**Question** How do the centre-of-mass position and momentum equations preserve physical quantities during a merge? 
Responce:
``totalMass = a.mass + b.mass``  is calculated from the original masses before modifiying the either body. Merged position is calculated as a centre of mass (``a.position * a.mass + b.position * b.mass) / totalMass;``) and merged velocity as momentum-weighted velocity (``a.velocity * a.mass + b.velocity * b.mass) / totalMass;``). 

All equations preserve total mass and the total momentum across the merge. Survivor a is updated with the totalmass, mergedposition and mergedvelocity. The second body is retired by setting b.mass = 0.0 and zeroing its velocity so latter passes can ignore it. 


### Code Evidence: 
``const double totalMass = a.mass + b.mass;
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
  ``

## 6. Seeded random-number generation
**Question** Why is std::mt19937 with an explicit seed preferable to rand() for these experiments? 
Responce:
mt19937 is a deterministic Mersenne Twister generator, so calling it with the same seed and same sequence of calls will produce the same sequence of random numbers of random numbers each run. 

rand() is implementation-defined(RAND_MAX - "guaranteed that this is at least 32767") and offers no guarantee of reproducibility across different compilers and platforms. 

### Code Evidence: 
    std::mt19937 generator(seed);
    std::uniform_int_distribution<std::size_t> pick(0, bodies.size() - 1);


### Evidence from repeated run: 
``std::vector<body> first = {makeBody(2, 0, 1), makeBody(1, 0.5, -2),
                             makeBody(0, 0.2, 0)};``

run 1: attempted 10 valid 1 merged 1 active 1
run 2: attempted 10 valid 1 merged 1 active 1


## 7. Refactoring Summary 
**Question** Which responsibilities were moved out of main.cpp, and how does the new structure make the program easier to test? 

### Simulation responsibilities:
### Renderer responsibilities:
### Csv responsibilities: 
### Other Change: 

Code Evidence: 


## 8. Test evidence
Document at least four focused tests,  including zero or negative parameters, zero collision threshold, exact-threshold distance, retired bodies and deterministic seeded runs.

| zero or negative parameter (dt) | set deltaTime = 0 and checks validates() throw                                                                              | pass zero dt rejected |
| zero collision threshold        | CheckandMergCollision returns false even when bodies are at same position                                                   |pass zero threshold no merge  |
| Exact-Threshold distance        | distance == threshold merges correctly, checks mass, position, velocity and retired body                                    |pass exact threshold merge                       |
| Retired bodies                  | body with mass <= 0 is ignored                                                                                              |pass retired body ignored                       |
| Deterministic seeded runs       | same seed + same vector gives identical merged pairs, valid pairs and active-bodies. also verifies attempt are capped at 10 |run 1: attempted 10 valid 1 merged 1 active 1
run 2: attempted 10 valid 1 merged 1 active 1
pass attempts limited to 10
pass same seed same result|
	


## 9. Challenge and Resolution

### Challenge encourted:

### How i investigated it:
 
### Solution Implemented:

### Evidence that the solution works: 

## AI-use statement
Tool(s), purpose, and how suggestions were verified:



