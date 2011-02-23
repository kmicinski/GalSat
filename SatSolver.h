/*
 * file: SatSolver.h
 * Purpose: 
 * Toplevel description of the abstract SatSolver class.
 * 
 * Author: Kristopher Micinski
 * Date: November 7th, 2010
 */

#ifndef _GALSAT_SOLVER_H_
#define _GALSAT_SOLVER_H_

#include <sys/time.h>

#include "Clause.h"
#include "Problem.h"
#include "VariableAssignment.h"
#include "SolverInfo.h"
#include "GalSatResourceInfo.h"

class SatSolver
{
private:
  // Resource constraints and usage.
  //GalSatResourceInfo *mResourceConstraints;
  //GalSatResourceInfo *mResourceUsage;
  
protected:
  // The initial model
  VariableAssignment *mModel;

  // What is the problem on which we are working
  Problem *mProblem;

  //struct timeval_t nextStepTime;
  //struct timeval_t stepTime;
  
  // Print the progress that solver has made
  virtual void printSolverProgress() = 0;
  
  virtual void stepTimeChecker();
  
  unsigned long mNumSteps;
  unsigned long mCurSteps;

  virtual void recordBeginTime() { }
  virtual void recordEndTime() { }
  
  // Check if a certain clause is conflicting or not
  bool checkClauseConflicting(Clause *clause, VariableAssignment *assignment);
  
  void setModel(VariableAssignment *a = NULL) { mModel = a; }
  
public:
  SatSolver();
  
  // Get the information about this solver.
  virtual SolverInfo *getSolverInfo() = 0;
  
  // Set the resources for this solver.
  //virtual void setResourceConstraints(GalSatResourceInfo *resources) { mResourceConstraints = resources; }
  //virtual GalSatResourceInfo const *getResourceConstraints() { return mResourceConstraints; }
  
  // Set the problem for this solver to work on.
  virtual void setProblem(Problem *p) { mProblem = p; }
  virtual Problem const *getProblem() { return mProblem; }
  
  // Have we found a model yet?
  bool getModelFound() { return mModel != NULL; }
  
  virtual unsigned long long getNumClauses() { return mProblem->getNumClauses(); }
  
  // Set the number of steps between each time we print progress.
  virtual void setSteps(long numSteps) { mNumSteps = numSteps; }
   
  virtual void printProgress();
  
  virtual void beginAttemptSolve() = 0;
  
  virtual void printModel();
  
  // Make sure that our model is correct
  bool checkModel();
};

#endif

