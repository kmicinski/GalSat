/*
 * File: GSatSolver.h
 * 
 * Author: Kristopher Micinski
 * Date: November 19th, 2010
 */

#ifndef _GALSAT_GSAT_SOLVER_H_
#define _GALSAT_GSAT_SOLVER_H_

#include <set>

#include "Problem.h"
#include "SatSolver.h"
#include "EnumerativeSolver.h"

class GSatSolver : public EnumerativeSolver
{
private:
  void printProgress();
  
protected:
  unsigned long numFlips;
  
  unsigned long mMaxFlips;
  
  int best;
  
public:
  virtual ~GSatSolver();
  GSatSolver();
  
  // Overwrite the function to guess an initial variable assignment for the problem
  virtual void guessInitialAssignment();
  
  // How much will flipping some 
  virtual int calculateFlippingResult(unsigned long long varNum);
  
  // Step the solver one run forward
  virtual bool stepOnce();

  // Try to solve the problem using the GSat method
  virtual void beginAttemptSolve();
};

#endif

