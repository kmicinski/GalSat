/*
 * File: GSatSolver.h
 *
 * Author: Kristopher Micinski
 * Date: December 4th, 2010
 */

#ifndef _GALSAT_GSAT_PARTIAL_SOLVER_H_
#define _GALSAT_GSAT_PARTIAL_SOLVER_H_

#include "GSatSolver.h"

/*
 * Class: GSatPartialSolver
 * Purpose:
 * Extends GSatSolver to compute partial solutions to SAT using variable flipping...
 */
class GSatPartialSolver : public GSatSolver
{
protected:
  // The set of satisfied clauses
  set<Problem::ProblemClausePtr> mSatisfiedClauses;
  
public:
  virtual ~GSatPartialSolver();
  GSatPartialSolver();
  
  enum ClauseAssignmentValues {
    CLAUSE_CONFLICTING,
    CLAUSE_SATISFIED,
    CLAUSE_UNSATISFIED
  };
  
private:
  // The probability that upon entering the partial solver (when a first assignment is made) 
  // that a varible will be turned on (*not* unassigned)
  float mAssignmentProbability;
    
protected:
  virtual void guessInitialAssignment();
  
  // And hill climb
  virtual bool stepOnce();
  
  // Print the set of *satisfied* clauses
  virtual void printSatisfiedClauses();
  
  // Find out if we are done yet... Different than for the simpler case where we assign everything
  virtual bool evaluateComplete() { return ((float)mSatisfiedClauses.size() == mProblem->getNumClauses()); }
  
  // Calculat the which way to flip a variable to increase the fitness the most, along with fitness value itself
  pair<Literal::Polarity,float> calculateBestFlipDirection(unsigned long long varNum);
  
  // Recalculate the set of conflicting cluases
  virtual void recalculateConflictingClauses();
  
  // Check to see if a certain clause under some assignment is satisfied, conflicting, or unsatisfied
  ClauseAssignmentValues checkClauseState(Clause *clause, VariableAssignment *assignment);
  
  // Assign a variable some value and calculate the conflicts resulting from the change...
  virtual void assignAndCalculateConflicts(unsigned long variableNumber,Literal::Polarity polarity);
  
public:
  virtual void beginAttemptSolve();
  
  // A number in the range [0..1] representing approximately how close we are to a solution to the problem.
  // Should be written in such a way that higher values imply we are closer to a good solution, however 
  // obviously an open research area.
  //
  // Note: you can extend this solver to fit whatever fitness function you want into this place.
  // but make sure this is 1 iff the assignment is completely correct.
  // (otherwise the toplevel solver will complain at you).
  virtual float currentFitness();
};

#endif
