/*
 * Name: Problem.h
 * Implementation: Problem.cc
 * Purpose:
 * Definitions for the Problem definition in GalSat.
 *
 * Author: Kristopher Micinski
 * Date: November 1st, 2010
 */

#ifndef _GALSAT_PROBLEM_H_
#define _GALSAT_PROBLEM_H_

#include <vector>
#include <list>

using namespace std;

#include "ClauseAllocator.h"
#include "Clause.h"
#include "LiteralWatcherDatabase.h"

 /*
 * Class: Problem
 * Purpose:
 * Holds a list of clauses.
 */
class Problem
{
protected:
  ClauseAllocator *mAllocator;

  // How many literals are there in the clauses?
  // I.e., what is the maximum literal number.
  unsigned long long mNumVariables;

  // A list of clause pointers, corresponding to the problem clauses.
  vector<Clause *> mClauses;
  
  // The literal watcher lists: Implemented in LiteralWatcherDatabase.h
  LiteralWatcherDatabase *mLiteralWatcherDB;
  
  // The problem is unitialized if the literal watcher database is NULL...
  bool isProblemInitialized() { return mLiteralWatcherDB != NULL; }
  
public:
  virtual ~Problem();
  Problem(ClauseAllocator *allocator = NULL);
  
  typedef Clause *ProblemClausePtr;
  
  typedef vector<Clause *>::iterator ProblemClausePtrIterator;
  
  // Copy constructor and operator
  Problem(const Problem& problem);
  Problem& operator=(Problem &problem);
  
  LiteralWatcherDatabase *getLiteralWatcherDB() { return mLiteralWatcherDB; }
  
  // Attach a clause to this clause container.
  void attachClause(Clause *clause);

  // Get the number of clauses in the proble.
  unsigned long long const getNumClauses() { return mClauses.size(); }
  
  // Get the number of variables throughout the clause set.
  unsigned long long const getNumVariables() { return mNumVariables; }
  
  // Set the problem's clause allocator
  void setAllocator(ClauseAllocator *allocator) { mAllocator= allocator; }

  // Get the problem's associated clause allocator
  ClauseAllocator const *getAllocator() { return mAllocator; }
  
  ProblemClausePtrIterator getProblemClauses() { return mClauses.begin(); }
  
  ProblemClausePtrIterator getProblemClausesEnd() { return mClauses.end(); }

  // Get a clause based on the clause number in the database.
  //Clause *getClause(unsigned long long clauseNumber) { return mClauses[clauseNumber]; }
  
  // Clean the problem, remove all clauses, etc...
  virtual void cleanProblem();
  
  // Set up the problem. *Must* be called before using the Problem for anything interesting
  virtual void initProblem(unsigned long long numVariables);
};

#endif

