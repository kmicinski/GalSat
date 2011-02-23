/* 
 * File: SatSolver.cc
 * Purpose:
 * Implementation of high level SAT solver necessities.
 * 
 * Author: Kristopher Micinski
 * Date: November 7th, 2010
 */

#include <iostream>
#include <sys/time.h>

#include "SatSolver.h"
#include "GalSatResourceInfo.h"
#include "VariableAssignment.h"

using namespace std;

SatSolver::SatSolver()
{
  mModel = NULL;
  //mResourceConstraints = NULL;
  mProblem = NULL;
  mNumSteps = 10000;
}

/*
 * Name: SatSolver::stepTimeChecker()
 * Purpose:
 * Step another iteration and check to see if we should print resource info yet.
 */
void SatSolver::stepTimeChecker()
{
  if (mCurSteps > mNumSteps)
    {
      printProgress();
      mCurSteps = 0;
    }
  else
    {
      mCurSteps++;
    }
  return;
}

void SatSolver::printProgress()
{
  return;
}

/*
 * Function: SatSolver::checkClauseConflicting
 * Purpose:
 * Check to see if a certain clause is conflicting under some variable assignment.
 */
bool SatSolver::checkClauseConflicting(Clause *clause, VariableAssignment *assignment)
{
  Clause::LiteralIterator it = clause->getLiteralIterator();
  
  for (; it != clause->getLastLiteral(); it++)
    {
      Literal lit = *it;
      
      if (assignment->queryVariablePolarity(lit.getVariableNumber()) == lit.getPolarity()
	  && assignment->queryVariablePolarity(lit.getVariableNumber()) != Literal::POLARITY_UNASSIGNED)
	{
	  // We have a non-conflicting assignment
	  return false;
	}
    }
  
  return true;
}

/*
 * Function: SatSolver::printModel
 * Purpose:
 * Print out a model of the problem.
 */
void SatSolver::printModel()
{
  cout << "\nModel:\n\n[";
  
  cout << *mModel;
  cout << "]\n";
}

/*
 * Function: SatSolver::checkModel
 * Purpose:
 * Do a check to just make sure that our model is correct.
 */
bool SatSolver::checkModel()
{
  Problem::ProblemClausePtrIterator curClause = mProblem->getProblemClauses();
    
  while (curClause != mProblem->getProblemClausesEnd())
    {
      if (checkClauseConflicting(*curClause,mModel))
	{
	  return false;
	}
      curClause++;
    }
  
  return true;
}
