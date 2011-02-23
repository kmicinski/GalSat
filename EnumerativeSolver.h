/*
 * File: EnumerativeSolver.h
 * Purpose:
 * Simple implementation of an enumerative solver for GalSat
 * 
 * Author: Kristopher Micinski
 * Date: November 7th, 2010.
 */

#ifndef _GALSAT_ENUMERATIVE_SOLVER_H_
#define _GALSAT_ENUMERATIVE_SOLVER_H_

#include <set>

#include "Problem.h"
#include "SatSolver.h"

class EnumerativeSolver : public SatSolver
{
protected:
  SolverInfo mSolverInfo;
  
  // The assignment with which we are curently working
  VariableAssignment *mWorkingAssignment;
  
  // The set of currently conflicting clauses
  set<Problem::ProblemClausePtr> mConflictingClauses;
  
  virtual unsigned int getNumConflictingClauses() { return mConflictingClauses.size(); }
  
  // Calculate the set of conflicting clauses all over again
  virtual void recalculateConflictingClauses();
  
  // Guess at an initial assignment
  virtual void guessInitialAssignment();
  
  // Make one step in the enumerative search
  // Return true if there is more to go!
  virtual bool stepOnce();
  
  // Find out if we're done yet
  virtual bool evaluateComplete();

  // Utility function to flip a variable and see what new conflicts resulted.
  virtual void flipAndCalculateConflicts(unsigned long variableNumber);
  
public:
  ~EnumerativeSolver();
  EnumerativeSolver();
  
  // Begin to look for the solution to a problem.
  virtual void beginAttemptSolve();
  
  // Get the solver info from the solver.
  SolverInfo *getSolverInfo() { return &mSolverInfo; }

  virtual void setProblem(Problem *p);
  
  void printConflictingClauses();
  
  void printSolverProgress();
  
  // Returns a random float in the range [0..1]
  float newRandomFloat();
};

#endif

