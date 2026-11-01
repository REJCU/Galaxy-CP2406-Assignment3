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

### Code Evidence: 

## 5. Conservation during Merge 
**Question** How do the centre-of-mass position and momentum equations preserve physical quantities during a merge? 
Responce:

### Code Evidence: 

## 6. Seeded random-number generation
**Question** Why is std::mt19937 with an explicit seed preferable to rand() for these experiments? 
Responce:

### Code Evidence: 
### Evidence from repeated run: 

## 7. Refactoring Summary 
**Question** Which responsibilities were moved out of main.cpp, and how does the new structure make the program easier to test? 

### Simulation responsibilities:
### Renderer responsibilities:
### Csv responsibilities: 
### Other Change: 

Code Evidence: 

## 8. Test evidence
Document at least four focused tests,  including zero or negative parameters, zero collision threshold, exact-threshold distance, retired bodies and deterministic seeded runs.


## 9. Challenge and Resolution

### Challenge encourted:

### How i investigated it:
 
### Solution Implemented:

### Evidence that the solution works: 

## AI-use statement
Tool(s), purpose, and how suggestions were verified:



