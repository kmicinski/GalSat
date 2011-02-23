/*
 * File: Problem.cc
 * Header: Problem.h
 * Purpose:
 * Implementation of Problem class
 *
 * Author: Kristopher Micinski
 * Date: November 7th, 2010
 */

#include "Problem.h"

/*
 * Function: Problem::Problem
 * Purpose:
 * Initialize the problem to a safe state.
 */
Problem::Problem(ClauseAllocator *allocator)
{
  mAllocator = allocator;
  mNumVariables = 0;
  mLiteralWatcherDB = NULL;
  return;
}

/*
 * Function: Problem::attachClause
 * Purpose:
 * Attach a clause to this problem, note that this will assume that this
 * problem will now be the controller of the passed clause's memory.
 * 
 * So you might want to think instead about passing a copy of the clause you 
 * want if you don't want this clauses memory deallocated out from under you.
 *
 * Arguments:
 * clause -- The clause to attach to this problem.
 */
void Problem::attachClause(Clause *clause)
{
  if (!isProblemInitialized())
    {
      return;
    }
  
  // Tell this clause that we are the problem they should be associated with
  clause->setProblem(this);
    
  // Add this clause to the literal's watcher list.
  for (Clause::LiteralIterator i = clause->getLiteralIterator();
       i != clause->getLastLiteral(); i++)
    {
      Literal &lit = *i;
      mLiteralWatcherDB->addWatcher(lit, clause);
    }
    
  // Actually *put* the clause there.
  mClauses.push_back(clause);
  
  return;
}

/*
 * Function: Problem::~Problem
 * Purpose:
 * Deconstructor, make sure we delete all the clauses in this problem, 
 * so that the clauses can be returned to the clause allocator.
 */
Problem::~Problem()
{
  // Delete all clauses in the problem.
  for (vector<Clause *>::iterator i = mClauses.begin();
       i != mClauses.end(); i++)
    {
      Clause *p = *i;
      delete p;
    }
  
  return;
}

/*
 * Function: Problem::cleanProblem()
 * Purpose:
 * Clean up one problem instance and prepare this problem for another
 * instance. Conceptually, the problem will hold the list of 
 */
void Problem::cleanProblem()
{
  if (!isProblemInitialized())
    {
      // The problem should be clean...
    }
  
  delete mLiteralWatcherDB;
  
  for (vector<Clause *>::iterator i = mClauses.begin();
       i != mClauses.end(); i++)
    {
      Clause *c = *i;
      mAllocator->returnClauseToAllocator(c);
    }
}

/*
 * Function: Problem::initProblem()
 * Purpose:
 * Initialize the Problem instance for use. Specify how many clauses and the number of 
 * variables we expect to handle. We then set up all of the routines to handle this.
 * Arguments:
 * numClauses -- The number of clauses we expect in our problem.
 * numVariables -- The number of variables we expect in our problem.
 */
void Problem::initProblem(unsigned long long numVariables)
{
  if (mLiteralWatcherDB != NULL)
    {
      // User hasn't remembered to clean the problem...
      return;
    }
  else
    {
      mLiteralWatcherDB = new LiteralWatcherDatabase(numVariables);
      mNumVariables = numVariables;
    }
  return;
}

