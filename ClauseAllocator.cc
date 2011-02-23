/*
 *
 */

#include "ClauseAllocator.h"

#include <list>

/*
 * Function: ClauseAllocator::ClauseAllocator
 * Purpose:
 * Set up clause allocator.
 */
ClauseAllocator::ClauseAllocator()
{
  mNumClauses = 0;
  
  return;
}

/*
 * Function: ClauseAllocator::requestNewClause
 */
Clause *ClauseAllocator::requestNewClause(unsigned numVariables)
{
  Clause *newClause = new Clause(numVariables,mNumClauses++);
}

void ClauseAllocator::returnClauseToAllocator(Clause *clause)
{
  return;
}
