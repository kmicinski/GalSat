/*
 * File: GSatSolver.cc
 * Purpose:
 * Implementation of the GSat search procedure for GalSat
 *
 * Author: Kristopher Micinski
 * Date: November 19th, 2010
 * 
 */

#include <iostream>
#include <set>
#include <list>
#include <vector>
#include <cstdlib> // For rand()
#include "GSatSolver.h"
#include "Problem.h"
#include "util.h"

using namespace std;

GSatSolver::GSatSolver()
{
  mSolverInfo.mSolverName = "gsatsolver";
  mSolverInfo.mSolverDescription = "A solver based on the gsat hill climbing method";
  mSolverInfo.mSolver = this;
  mMaxFlips = 10000000; // Set a large number of max flips

  return;
}

GSatSolver::~GSatSolver()
{
  return;
}

/*
 * Function: GSatSolver::guessInitialAssignment
 * Purpose:
 * Come up with an initial assignment for the clause the user
 * gives to us.
 */
void GSatSolver::guessInitialAssignment()
{
  for (unsigned long long i = 0;
       i < mProblem->getNumVariables(); i++)
    {
      if ((rand() % 2) == 0)
	mWorkingAssignment->assignLiteral(i,Literal::POLARITY_FALSE);
      else
	mWorkingAssignment->assignLiteral(i,Literal::POLARITY_TRUE);
    }
  
  return;
}

/*
 * Function: GSatSolver::calculateFlippingResult
 * Purpose:
 * In the hill climbing scheme, we need to be able to know when some 
 * variable will help us more than another variable. To do this we need 
 * to be able to find out how many more clauses would be satisfied if we
 * flipped some variable. This function calculates, given an arbitrary 
 * variable, how many more clauses would be satisfied than in the current 
 * variable assignment.
 * Arguments
 * varNum -- The variable number to investigate
 */
int GSatSolver::calculateFlippingResult(unsigned long long varNum)
{
  // How much have we changed our assignment?
  int delta = 0;
  
  Clause::ClausePtrIterator potentialClauses = mProblem->getLiteralWatcherDB()->getWatchingClauses(varNum);
  
  // For all potentially changed clauses
  for (; potentialClauses != mProblem->getLiteralWatcherDB()->getLastWatchingClause(varNum);potentialClauses++)
    {
      
      Clause *i = *potentialClauses;
      //cout << (*i) << endl;
      
      Literal::Polarity polarityInClause = Literal::POLARITY_UNASSIGNED;
      bool varCorrect = false;
      unsigned numVariablesCorrect = 0;
      
      // For all literals in that potentially changed clause...
      for (Clause::LiteralIterator lits = i->getLiteralIterator();
	   lits != i->getLastLiteral(); lits++)
	{
	  Literal &literal = *lits;
	  
	  if (literal.getVariableNumber() == varNum)
	    {
	      // Found the variable we want...
	      if (polarityInClause != Literal::POLARITY_UNASSIGNED)
		{
		  // Trivially true... Because we've got a true and a false both true...
		  goto end_of_clause_loop;
		}
	      
	      polarityInClause = literal.getPolarity();
	      
	      if (mWorkingAssignment->queryVariablePolarity(varNum)
		  == literal.getPolarity())
		{
		  varCorrect = true;
		  numVariablesCorrect++;
		}
	    }
	  else
	    {
	      // Not the variable we're looking for
	      if (mWorkingAssignment->queryVariablePolarity(literal.getVariableNumber())
		  == literal.getPolarity())
		{
		  numVariablesCorrect++;
		}
	    }
	  
	}
      
      if (numVariablesCorrect == 0)
	{
	  // *No* variables are correct, since the variable is in our clause, we know that when we switch it
	  // then this clause will become satisfied.
	  delta++;
	}
      else if (numVariablesCorrect > 1)
	{
	  // Nothing changes... Changing the value of the variable will not hurt us *or* help us.
	  // Because we've already got *another* variable assigned that will render the clause true.
	  ;
	}
      else if (numVariablesCorrect == 1 && varCorrect)
	{
	  // The variable was correct, and also the *only* correct variable in the clause,
	  // so when we switch it, the clause will become unsatisfied...
	  // Actually, in the case of GSat, we really just want to walk up without caring how 
	  // many clauses we are affecting in a unsatisfying sense.
	  //delta--;
	}
    end_of_clause_loop:
      ;
    }
  
  return delta;
}

/*
 * Function: GSatSolver::stepOnce()
 * Purpose:
 * Make one flip of a variable which will send us up the hill. We 
 * have to be a bit careful to not deterministically choose only 
 * one value (i.e., don't just choose the first best assignment) 
 * so we don't get caught in a loop. To avoid this, we use a list 
 * variables which are equally helpful and keep them in our list.
 *
 */
bool GSatSolver::stepOnce()
{
  int bestDelta = calculateFlippingResult(0);
  vector<unsigned long long> bestVariables;
  unsigned long long pickedVariable;
  list<unsigned long long>::iterator i;
  unsigned long long bestVariable;
  
  if ((rand() % 1000) < 100)
    {
      int numVars = mProblem->getNumVariables();
      pickedVariable = rand() % numVars;
      // Pick a random variable...
      //      if (Environment.verbose)
      //	{
      //	  cout << "Picking random variable " << pickedVariable << endl;
	  //	}

      flipAndCalculateConflicts(pickedVariable);
      return false;
    }
  
  bestVariables.push_back(0);
  
  // Calculate the best variable to flip.
  for (unsigned long long i = 1;
       i < mProblem->getNumVariables(); i++)
    {
      int delta = calculateFlippingResult(i);
      //      cout << delta;
      //cout << endl;
      if (delta == bestDelta)
	{
	  // Add this to the list...
	  bestVariables.push_back(i);
	}
      if (delta > bestDelta)
	{
	  // Clear the old list, we've got a new *higher* delta...
	  bestVariables.clear();
	  bestDelta = delta;
	  bestVariables.push_back(i);
	}
    }
  
  // Now choose a variable...
  pickedVariable = bestVariables[rand() % bestVariables.size()];
  best = bestVariables.size();
  
  //  pickedVariable = rand() % (bestVariables.size() + 1);
  
  
  /*for (i = bestVariables.begin();
       pickedVariable > 0; i++)
    {
      pickedVariable--;
    }
    
  bestVariable = *i;
  */
  
  if (Environment.verbose)
  {
        cout << "Flipping variable " << pickedVariable << " results in "
  	   << bestDelta << " more satisfying assignments, flipping it..."
  	   << endl;
  }
  
  flipAndCalculateConflicts(pickedVariable);
  
  return false;
  
}

void GSatSolver::beginAttemptSolve()
{
  cout << "Attempting solution via GSat solver...";
  
  // Come up with an initial assignment
  guessInitialAssignment();
  
  // Fill the conflicting clause set with conflicting clauses
  EnumerativeSolver::recalculateConflictingClauses();
  
  numFlips = 0;
  
  if (Environment.verbose) {
    cout << "Starting assignment:\n";
    cout << *mWorkingAssignment << endl;
  }
  
  //  cout << "Conflicting clauses:" << endl;
  //printConflictingClauses();
  

  while (!evaluateComplete()) {
    if (--mMaxFlips == 0)
      {
	cout << "Maximum number of steps reached... no assignment found..." << endl;
	return;
      }
    
    if (Environment.verbose) 
      {
	cout << "Stepping once..." << endl;
      }
    
    stepOnce();
    stepTimeChecker();
    numFlips++;
    if (Environment.verbose) {
      //  cout << "\nCurrent assignment:\n";
      //cout << *mWorkingAssignment;
      //cout << "Conflicting clauses:" << endl;
      printConflictingClauses();
    }
  }
  
  return;
}

void GSatSolver::printProgress()
{
  cout << "Flips: " << numFlips << ". Conflicting Clauses: " << mConflictingClauses.size()
       << " best: " << best << endl;
 
  // Model?
  return;
}
