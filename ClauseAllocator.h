/*
 *
 */

#ifndef _GALSAT_CLAUSE_ALLOCATOR_H_
#define _GALSAT_CLAUSE_ALLOCATOR_H_

#include <vector>

#include "Clause.h"

using namespace std;

class ClauseAllocator
{
private:
  // The list of all allocated clauses
  list<Clause *> mAllAllocatedClauses;
  
  // The pool of *free* clauses
  list<Clause *> mFreeClauses;
  
  unsigned long long mNumClauses;
  
  unsigned long long mMaxClauses;
  
public:
  ClauseAllocator();
  
  Clause *requestNewClause(unsigned numLiterals);
  
  // Deallocate a clause... I.e., clean up the clause and return it to the pool
  // of free clauses.
  void returnClauseToAllocator(Clause *c);
  
  void setMaxClauses(unsigned long long clauses) { mMaxClauses = clauses; }
  
  unsigned long long getMaxClauses() { return mMaxClauses; }
};

#endif
