/*
 * File: EnumerativeSolver.cc
 * Purpose:
 * Simple implementaiton of an enumerative solver for GalSat
 *
 * Author: Kristopher Micinski
 * Date: November 7th, 2010
 *
 * Implementation details:
 * The enumerative solver uses an enumerative technique to find
 * out if a formula is satisfiable or not. We initially "guess"
 * at a variable assignment that may or may not work. We then
 * run through all clauses in the problem and find which clauses
 * are conflicting. After this, we check to see how many clauses
 * are on our conflicting list. If there are none we are finished.
 * However, the next step involves mutating the assignment so that
 * we cover all possilbe variable assignments. We use the Gray code
 * so that all possible assignments are covered. This allows us
 * to flip one variable at a time. If we flip a single variable,
 * we don't have to recalculate all of the conflicting clauses,
 * we simply have to look at the variable watcher lists and see
 * which clauses *might* have changed their status. If a clause
 * becomes unsatisfied, we put it in the conflict set. However,
 * if a clause previously not in the conflict becomes conflicting,
 * we reomove it from the conflict set. After each assignment,
 * we check the size of the conflict set and see if it is zero.
 * If we exhaust our gray code, we then return Satisfiable.
 */

#include <iostream>
#include <stdlib.h>
#include "EnumerativeSolver.h"
#include "Problem.h"
#include "util.h"

using namespace std;

EnumerativeSolver::EnumerativeSolver()
  : mWorkingAssignment(NULL)
{
  mSolverInfo.mSolverName = "enumsolver";
  mSolverInfo.mSolverDescription = "An enumerative solver";
  mSolverInfo.mSolver = this;
  
  return;
}

EnumerativeSolver::~EnumerativeSolver()
{
  mConflictingClauses.clear();
  
  return;
}


/*
 * Function: EnumerativeSolver::beginAttemptSolve()
 * Purpose:
 *
 */
void EnumerativeSolver::beginAttemptSolve()
{
  if (mWorkingAssignment == NULL)
    {
      // User hasn't properly initialized the solver!
      cerr << "Solver has not been properly initialized!\n";
      return;
    }
  
  cout << "Beginning search for model using enumerative method...\n";
  
  guessInitialAssignment();
  recalculateConflictingClauses();
  
  if (Environment.verbose)
    {
      cout << "Initial assignment:" << endl;
      cout << *mWorkingAssignment << endl;
    }
  
  printConflictingClauses();
  
  // While we still have more possibilities to try
  while (stepOnce() != false)
    {
      if (Environment.verbose)
	{
	  cout << *mWorkingAssignment << endl;
	  printConflictingClauses();
	}
      if(evaluateComplete())
	{
	  return;
	}
    }
  return;
}

bool EnumerativeSolver::evaluateComplete()
{
  if (mConflictingClauses.size() == 0)
    {
      mModel = mWorkingAssignment;
      return true;
    }
  else
    {
      return false;
    }
}

/*
 * Function: EnumerativeSolver::guessInitialAssignment()
 * Purpose:
 * Guess an initial variable assignment.
 */
void EnumerativeSolver::guessInitialAssignment()
{
  for (unsigned long long i = 0;
       i < mProblem->getNumVariables(); i++)
    {
      mWorkingAssignment->assignLiteral(i,Literal::POLARITY_FALSE);
    }
  
  return;
}

/*
 * Function: ()
 */
void EnumerativeSolver::printConflictingClauses()
{
  cout << "Conflicting clauses:" << endl;
  
  for (set<Problem::ProblemClausePtr>::iterator i = mConflictingClauses.begin();
       i != mConflictingClauses.end(); i++)
    {
      cout << **i << endl;
    }
  return;
}

/*
 * Function: EnumerativeSolver::stepOnce()
 * Purpose:
 * Step through one iteration of the enumerative
 * solver.
 * 
 * Implementation note:
 * Simply add one, the Gray sequence is too hard to implement currently,
 * but I *did* order Knuth's book on enumerating set's, so we're all good
 * in a few years or so.
 */
bool EnumerativeSolver::stepOnce()
{
  
  bool carry = false;
  unsigned long long varNumber = 0;
  if (Environment.verbose)
    {
      cout << "Stepping to next solution..." << endl;
    }
  
  for (;
       varNumber < mProblem->getNumVariables();
       varNumber++)
    {
      if (carry == false)
	{
	  // False carry in...
	  // We are on the first variable
	  if (mWorkingAssignment->queryVariablePolarity(varNumber == Literal::POLARITY_FALSE))
	    {
	      // Assign this one true and be done with it.
	      flipAndCalculateConflicts(varNumber);
	      break;
	    }
	  else
	    {
	      flipAndCalculateConflicts(varNumber);
	      carry = true;
	    }
	}
      else
	{
	  if (mWorkingAssignment->queryVariablePolarity(varNumber) == Literal::POLARITY_FALSE)
	    {
	      // Our working variable is false, just flip it to true and call us done.
	      flipAndCalculateConflicts(varNumber);
	      break;
	    }
	  else
	    {
	      // Working variable is true... flip it and keep going
	      flipAndCalculateConflicts(varNumber);
	      
	      carry = true;
	    }
	}
    }
  return varNumber != mProblem->getNumVariables();
}

/*
 * Function: EnumerativeSolver::flipAndCalculateConflicts
 * Purpose:
 * Flip a variable in the working assignment and see what new contradictions it 
 * introduces.
 */
void EnumerativeSolver::flipAndCalculateConflicts(unsigned long variableNumber)
{
  Clause::ClausePtrIterator watchingClause;
  
  if (mWorkingAssignment->queryVariablePolarity(variableNumber) == Literal::POLARITY_FALSE)
    {
      // Switch to true...
      mWorkingAssignment->assignLiteral(variableNumber,Literal::POLARITY_TRUE);
    }
  else
    {
      mWorkingAssignment->assignLiteral(variableNumber,Literal::POLARITY_FALSE);
    }
  
  LiteralWatcherDatabase *db = mProblem->getLiteralWatcherDB();
  
  watchingClause = mProblem->getLiteralWatcherDB()->getWatchingClauses(variableNumber);
  
  for (; watchingClause != mProblem->getLiteralWatcherDB()->getLastWatchingClause(variableNumber);
       watchingClause++)
    {
      Clause *curClause = *watchingClause;
      
      if (checkClauseConflicting(curClause,mWorkingAssignment))
	{
	  // CurClause is conflicting, add it to the conflict set
	  mConflictingClauses.insert(curClause);
	}
      else
	{
	  // Not conflicting, remove it from the conflicting set
	  mConflictingClauses.erase(curClause);
	}
    }
}

/*
 * Function: EnumerativeSolver::recalculateConflictingClauses()
 * Purpose:
 * When we first start, we don't know which clauses will be conflicting.
 * We can only make an initial assignment, i.e., all 0s. However,
 * we then need to calculate how many conflicting clauses we have. 
 * This function implements that functionality.
 * Comments:
 * Note that this is O(n*log(n)) because the call to insert on a set takes
 * log(n) take.
 */
void EnumerativeSolver::recalculateConflictingClauses()
{
  Problem::ProblemClausePtrIterator it = mProblem->getProblemClauses();
  
  for (; it != mProblem->getProblemClausesEnd(); it++)
    {
      Clause *clause = *it;
      
      if (checkClauseConflicting(clause,mWorkingAssignment))
	{
	  // Clause is conflicting, add it to the conflict set.
	  mConflictingClauses.insert(clause);
	}
      else if (1) //checkClauseSatisfied(clause,mWorkingAssignment))
	{
	  // ...
	}
    }
  
  return;
}

/*
 *n
 */
void EnumerativeSolver::printSolverProgress()
{
  // ...
  return;
}

/*
 * Function: EnumerativeSolver::setProblem
 * Purpose:
 * Set the problem up for the solver.
 */
void EnumerativeSolver::setProblem(Problem *p)
{
  // First call our super method.
  SatSolver::setProblem(p);
  
  mWorkingAssignment = new VariableAssignment(p->getNumVariables());
  
  return;
}

/* 
 * Function: EnumerativeSolver::newRandomFloat
 * Purpose:
 * Generate a new random floating point number in the range [0..1]
 * 
 */
float EnumerativeSolver::newRandomFloat()
{
  int randNum = rand() % 10000000;
  
  return ((float)randNum)/10000000.0;


}
