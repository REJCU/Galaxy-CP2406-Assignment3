# Code Review

## Student and Project Details

**Student Name:** Ryan Ericson
**Student ID:** 14220594
**Project Options:** Barnes-Hut Galaxy Simulation

Answer each question from the assessment in your own words and refer to specific functions or lines in your project.

## 1. Pointer member access
**Question:** For body* a. what does a->position.x mean, and how it it related to (*.a).position.x?

Responce:

Code Evidence: 


## 2. Octree child selection
**Question** How does Bhtree::insertIntoChild decide which child octant recieves a body? What boundary behaviour should a reviewer notice? 
Responce:

Code Evidence: 

## 3. Distinct collision indices
**Question** Why must a collision attempt select two distinct indices, and what can go wrong if the same body is selected twice? 
Responce:

Code Evidence: 



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



