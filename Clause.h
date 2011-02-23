/*
 * File: Clause.h
 * Purpose:
 * Implementation of cluases for GalSat.
 * 
 * Author: Kristopher Micinski
 * Date: November 1st, 2010.
 */

#ifndef _GALSAT_CLAUSE_H_
#define _GALSAT_CLAUSE_H_

#include <vector>
#include <list>

using namespace std;

#include "Literal.h"
#include "LiteralWatcherDatabase.h"

// We need to predeclare this
class ClauseAllocator;
class Problem;

class Clause
{
private:
  // Is this clause currently allocated?
  // Note: This doesn't say whether or not the clause is currently allocated in memory.
  // Rather, it says whether it is currently allocated to a problem, or just sitting
  // in a free pool.
  bool mCurrentlyAllocated;
  
  // A pointer to this clause's allocator
  ClauseAllocator *mAllocator;
  
  // The clause number, used to distinguish this clause in a large database of clauses.
  unsigned long long mClauseNumber;
  
  // The number of literals the clause has.
  unsigned long mNumLiterals;
  
  // The set of literals in the clause.
  std::vector<Literal> mLiterals;
  
  // The (possibly) associated watcher database for this clause
  LiteralWatcherDatabase *mLitWatcherDB;
  
  // this clause's reference count.
  unsigned long mRefCount;
  
  // Is this clause a problem clause (as opposed to some other clause, a learned clause, etc...)
  bool mIsProblemClause;
  
  Problem *mProblem;
  
  // Make the copy constructor private, Clauses must be allocated from the clause pool.
  Clause(const Clause& clause);
  
  friend class ClauseAllocator;
  
 public:
  virtual ~Clause() { }
  
  typedef list<Clause *>::iterator ClausePtrIterator;
  typedef vector<Literal>::iterator LiteralIterator;
  
  // Create a new clause with a certain number of variables and a certain number.
  Clause (unsigned long numVariables, unsigned long long clauseNumber);
  
  Clause& operator=(Clause &clause);
  Clause& operator=(list<Literal> &literals);
  
  friend ostream& operator<<(ostream &stream, Clause &c);
  
  unsigned long long getClauseNumber() const { return mClauseNumber; }
    
  // Return true if this clause is currently being used, or false if it is
  // just sitting in a pool.
  bool const getCurrentlyAllocated() { return mCurrentlyAllocated; }
    
  // Add a literal to the clausen
  virtual void addLiteral(Literal literal);
  
  virtual unsigned long const getNumLiterals() { return mNumLiterals; }
  
  virtual void inline setLiteral(unsigned long literalNumber, Literal::Polarity polarity);
  virtual Literal const getLiteral (unsigned long literalNumber);
  
  // Get the polarity of some literal in the clause
  virtual Literal::Polarity const getLiteralValue (unsigned long literalNumber) { }
  
  void setProblemClause(bool isProblemClause = true) { mIsProblemClause = isProblemClause; }
  bool getProblemClause() { return mIsProblemClause; }
  
  void setProblem(Problem *p) { mProblem = p; }
  Problem *getProblem() { return mProblem; }
  
  // Clear this clause, bring it back to the default state
  void clearClause();
  
  // Remove this clause from its Literals' watcher lists.
  void removeFromWatcherLists();
  
  LiteralIterator getLiteralIterator() { return mLiterals.begin(); }
  LiteralIterator getLastLiteral() { return mLiterals.end(); }
  
  void setLiteralWatcherDatabase(LiteralWatcherDatabase *db) { mLitWatcherDB = db; }
  LiteralWatcherDatabase *getLiteralWatcherDatabase() { return mLitWatcherDB; }
};

#endif

