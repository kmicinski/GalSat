/*
 * File: GSatPartialSolver.cc
 * Purpose:
 * Implementation of GSAT-Partial procedure for GalSAT
 * 
 * Author: Kristhoper Micinski
 * Date: December 4th, 2010
 *
 */

#include <stdlib.h>
#include <vector>
#include <utility>
#include <iostream>

#include "GSatPartialSolver.h"
#include "util.h"

using namespace std;

GSatPartialSolver::GSatPartialSolver()
{
  mSolverInfo.mSolverName = "gsatpartialsolver";
  mSolverInfo.mSolverDescription = "A solver based on partial gsat assignments and hill climbing";
  mSolverInfo.mSolver = this;
  mMaxFlips = 10000000;
  mAssignmentProbability = 0.7;
  
  return;
}


GSatPartialSolver::~GSatPartialSolver()
{
  return;
}

/*
 * Function: GSatPartialSolver::guessInitialAssignment
 * Purpose:
 * Come up with an initial assignment.
 */
void GSatPartialSolver::guessInitialAssignment()
{
  for (unsigned long long i = 0;
       i < mProblem->getNumVariables(); i++)
    {
      float random = newRandomFloat();
      
      if (random <= mAssignmentProbability) {
	int randNum = rand() % 2;
	
	if (randNum) 
	  {
	    mWorkingAssignment->assignLiteral(i,Literal::POLARITY_TRUE);
	  }
	else
	  {
	    mWorkingAssignment->assignLiteral(i,Literal::POLARITY_FALSE);
	  }
      }
      else
	{
	  mWorkingAssignment->assignLiteral(i,Literal::POLARITY_UNASSIGNED);
	}
    }
}

/*
 * Function: GSatPartialSolver::stepOnce()
 * Purpose:
 * Make one step toward the better solution of the problem...
 */
bool GSatPartialSolver::stepOnce()
{
  float bestDelta;
  pair<Literal::Polarity,float> pickedAssignment;
  vector<pair<Literal::Polarity,float> > bestAssignments;
  vector<unsigned long long> bestVariables;
  unsigned long long pickedVariable;
  
  pair<Literal::Polarity,float> first = calculateBestFlipDirection(0);
  
  bestAssignments.push_back(first);
  bestDelta = first.second;
  bestVariables.push_back(0);
  
  if ((rand() % 1000) < 300)
    {
      int numVars = mProblem->getNumVariables();
      unsigned long long pickedVariable = rand() % numVars;
      int picked = rand() % 3;
      
      switch(picked) 
	{
	case 0:
	  assignAndCalculateConflicts(pickedVariable,Literal::POLARITY_FALSE);
	  break;
	case 1:
	  assignAndCalculateConflicts(pickedVariable,Literal::POLARITY_TRUE);
	  break;
	case 2:
	  assignAndCalculateConflicts(pickedVariable,Literal::POLARITY_UNASSIGNED);
	  break;
	}
      return false;
    }
  
  for (unsigned long long i = 1;
       i < mProblem->getNumVariables(); i++)
    {
      pair<Literal::Polarity,float> calculated = calculateBestFlipDirection(i);
      
      if (calculated.second == bestDelta) {
	bestAssignments.push_back(calculated);
	bestVariables.push_back(i);
      }
      if (calculated.second > bestDelta)
	{
	  bestAssignments.clear();
	  bestVariables.clear();
	  bestDelta = calculated.second;
	  bestAssignments.push_back(calculated);
	  bestVariables.push_back(i);
	}
    }
  
  int pick = rand() % bestAssignments.size();
  
  pickedAssignment = bestAssignments[pick];
  pickedVariable = bestVariables[pick];
  best = bestAssignments.size();
  
  if (Environment.verbose)
    {
      cout << "Flipping " << pickedVariable << " to ";
      switch (pickedAssignment.first) {
      case Literal::POLARITY_UNASSIGNED:
	cout << "unassigned";
	break;
      case Literal::POLARITY_FALSE:
	cout << "false";
	break;
      case Literal::POLARITY_TRUE:
	cout << "true";
	break;
      }
      cout << " results in a gain of " << bestDelta << " increase in fitness... doing it...\n";
    }
  
  assignAndCalculateConflicts(pickedVariable,pickedAssignment.first);
  
  return false;
}

/*
 * Function: GSatPartialSolver::calculateBestFlipDirection
 * Purpose:
 * Calculate the best direction to increase the fitness of the population.
 */
pair<Literal::Polarity,float> GSatPartialSolver::calculateBestFlipDirection(unsigned long long varNum)
{
  float bestTrueAssignDirection;
  float bestFalseAssignDirection;
  float bestUnassignDirection;

  float curFitness = currentFitness();
  
  switch(mWorkingAssignment->queryVariablePolarity(varNum))
    {
    case Literal::POLARITY_TRUE:
      bestTrueAssignDirection = 0.0;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_FALSE);
      bestFalseAssignDirection = currentFitness() - curFitness;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_TRUE);
      assignAndCalculateConflicts(varNum,Literal::POLARITY_UNASSIGNED);
      bestUnassignDirection = currentFitness() - curFitness;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_TRUE);
      break;
    case Literal::POLARITY_FALSE:
      bestFalseAssignDirection = 0.0;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_TRUE);
      bestTrueAssignDirection = currentFitness() - curFitness;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_FALSE);
      assignAndCalculateConflicts(varNum,Literal::POLARITY_UNASSIGNED);
      bestUnassignDirection = currentFitness() - curFitness;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_FALSE);
      break;
    case Literal::POLARITY_UNASSIGNED:
      bestUnassignDirection = 0.0;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_TRUE);
      bestTrueAssignDirection = currentFitness() - curFitness;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_UNASSIGNED);
      assignAndCalculateConflicts(varNum,Literal::POLARITY_FALSE);
      bestFalseAssignDirection = currentFitness() - curFitness;
      assignAndCalculateConflicts(varNum,Literal::POLARITY_UNASSIGNED);
      break;
    }
  
  if (bestTrueAssignDirection >= bestFalseAssignDirection) {
    if (bestTrueAssignDirection >= bestUnassignDirection) {
      return pair<Literal::Polarity,float>(Literal::POLARITY_TRUE,bestTrueAssignDirection);
    } else {
      return pair<Literal::Polarity,float>(Literal::POLARITY_FALSE,bestFalseAssignDirection);
    }
  } else {
    if (bestFalseAssignDirection >= bestUnassignDirection) {
      return pair<Literal::Polarity,float>(Literal::POLARITY_FALSE,bestFalseAssignDirection);
    } else {
      return pair<Literal::Polarity,float>(Literal::POLARITY_UNASSIGNED,bestUnassignDirection);
    }
  }
}

/*
 * Function: GSatPartialSolver::printSatisfiedClauses
 * Purpose:
 * Print the satisfied set of clauses for the dataset...
 */
void GSatPartialSolver::printSatisfiedClauses()
{
  cout << "Satisfied clauses:" << endl;
  
  for (set<Problem::ProblemClausePtr>::iterator i = mSatisfiedClauses.begin();
       i != mSatisfiedClauses.end(); i++)
    {
      cout << **i << endl;
    }
  return;
}

/*
 * Function: GSatSolver:;currentFitness()
 * Purpose:
 * Extendable function (virutal) to calculate the current fitness of a variable assignment.
 * Lets us play around with variable assignments.
 */
float GSatPartialSolver::currentFitness()
{
  return ((float)mSatisfiedClauses.size())/(mProblem->getNumClauses());
}

void GSatPartialSolver::beginAttemptSolve()
{
  cout << "Attempting solution via GSat partial solver...";
  
  // Come up with an initial assignment
  guessInitialAssignment();

  if (Environment.verbose) {
    cout << "Starting assignment:\n";
    cout << *mWorkingAssignment << endl;
  }
  
  // First find out our initial sets of conflicting and correctly guessed clauses...
  recalculateConflictingClauses();
  
  numFlips = 0;
  
  if (Environment.verbose) {
    printConflictingClauses();
    cout << endl;
    printSatisfiedClauses();
    cout << endl;
  }
  
  while (!evaluateComplete()) {
    if (--mMaxFlips == 0)
      {
	cout << "Maximum number of steps reached... no assignment found..." << endl;
	//Return;
	break;
      }
    
    if (Environment.verbose)
      {
	cout << "Stepping once..." << endl;
      }
    
    stepOnce();
        
    stepTimeChecker();
    numFlips++;
    if (Environment.verbose)
      {
	cout << *mWorkingAssignment << "\n";
	printConflictingClauses();
	cout << endl;
	printSatisfiedClauses();
	cout << endl;
      }
  }
  
  // Found a model!
  mModel = mWorkingAssignment;
  
  return;
}

/*
 * Function: SatSolver::checkClauseState
 * Purpose:
 * Check to find out if this clause is conflicting, satisfied, or neither...
 */
GSatPartialSolver::ClauseAssignmentValues GSatPartialSolver::checkClauseState(Clause *clause, VariableAssignment *assignment)
{
  Clause::LiteralIterator it = clause->getLiteralIterator();
  
  bool hadUnassignedLiterals = false;
  
  for (; it != clause->getLastLiteral(); it++)
    {
      Literal lit = *it;
      
      if (assignment->queryVariablePolarity(lit.getVariableNumber()) == Literal::POLARITY_UNASSIGNED)
	{
	  hadUnassignedLiterals = true;
	}
      
      if (assignment->queryVariablePolarity(lit.getVariableNumber()) == lit.getPolarity()
	  && assignment->queryVariablePolarity(lit.getVariableNumber()) != Literal::POLARITY_UNASSIGNED)
	{
	  // We have a satisfying assignment
	  return CLAUSE_SATISFIED;
	}
    }
  
  return (hadUnassignedLiterals ? CLAUSE_UNSATISFIED: CLAUSE_CONFLICTING);
}

/*
 * Function: GSatPartialSolver::recalculateConflictingClauses()
 * Purpose:
 * Does the same thing as recalculateConflictingClauses on 
 * the regular enumerative solver, except this one also calculates
 * satisfied clauses.
 */
void GSatPartialSolver::recalculateConflictingClauses()
{
  Problem::ProblemClausePtrIterator it = mProblem->getProblemClauses();
  
  mConflictingClauses.clear();
  mSatisfiedClauses.clear();
  
  //cout << endl;
  
  for (; it != mProblem->getProblemClausesEnd(); it++)
    {
      Clause *clause = *it;
      
      //cout << *clause << endl;
      
      switch (checkClauseState(clause,mWorkingAssignment))
	{
	case CLAUSE_CONFLICTING:
	  mConflictingClauses.insert(clause);
	  break;
	case CLAUSE_SATISFIED:
	  mSatisfiedClauses.insert(clause);
	  break;
	case CLAUSE_UNSATISFIED:
	  // Do nothing...
	  break;
	}
    }
  
  return;
}

/*
 * Function: EnumerativeSolver::assignAndCalculateConflicts
 * Purpose:
 * Turn a clause on/off and calculate conflicts
 * introduces.
 */
void GSatPartialSolver::assignAndCalculateConflicts(unsigned long variableNumber, Literal::Polarity polarity)
{
  Clause::ClausePtrIterator watchingClause;
  
  mWorkingAssignment->assignLiteral(variableNumber,polarity);
    
  LiteralWatcherDatabase *db = mProblem->getLiteralWatcherDB();
  
  watchingClause = mProblem->getLiteralWatcherDB()->getWatchingClauses(variableNumber);
  
  for (; watchingClause != mProblem->getLiteralWatcherDB()->getLastWatchingClause(variableNumber);
       watchingClause++)
    {
      Clause *curClause = *watchingClause;
      
      switch (checkClauseState(curClause,mWorkingAssignment))
	{
	case CLAUSE_CONFLICTING:
	  mSatisfiedClauses.erase(curClause);
	  mConflictingClauses.insert(curClause);
	  break;
	case CLAUSE_SATISFIED:
	  mSatisfiedClauses.insert(curClause);
	  mConflictingClauses.erase(curClause);
	  break;
	case CLAUSE_UNSATISFIED:
	  mSatisfiedClauses.erase(curClause);
	  mConflictingClauses.erase(curClause);
	  break;
	}
    }
  
  return;
}
