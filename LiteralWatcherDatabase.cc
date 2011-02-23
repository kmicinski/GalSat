/*
 * File: LiteralWatcherDatabase.cc
 * Purpose:
 * Implementation of watched literals.
 * 
 * Author: Kristoper Micinski
 * Date: November 10th, 2010
 */

#include "LiteralWatcherDatabase.h"
#include "Clause.h"

LiteralWatcherDatabase::LiteralWatcherDatabase(unsigned long long NumLiterals)
{
  mLiteralWatchers.resize(NumLiterals);
}

/*
 * Function: LiteralWatcherDatabase::~LiteralWatcherDatabase
 * Purpose:
 * Implementation of literal watcher database.
 * 
 */
LiteralWatcherDatabase::~LiteralWatcherDatabase()
{
  for (vector<list<Clause *> >::iterator i = mLiteralWatchers.begin();
       i != mLiteralWatchers.end(); i++)
    {
      list<Clause *> &ilist = *i;
      
      ilist.clear();
    }
  
  mLiteralWatchers.clear();
  
  return;
}

/*
 * Function: LiteralWatcherDatabase::getWatchers
 * Purpose:
 * Return a set of clauses watching a certain variable.
 * 
 * Arguments:
 * variable -- The variable number.
 */
Clause::ClausePtrIterator LiteralWatcherDatabase::getWatchingClauses(unsigned long variable)
{
  return mLiteralWatchers[variable].begin();
}

Clause::ClausePtrIterator LiteralWatcherDatabase::getWatchingClauses(Literal &lit)
{
  return mLiteralWatchers[lit.getVariableNumber()].begin();
}

Clause::ClausePtrIterator LiteralWatcherDatabase::getLastWatchingClause(unsigned long variable)
{
  return mLiteralWatchers[variable].end();
}

Clause::ClausePtrIterator LiteralWatcherDatabase::getLastWatchingClause(Literal &lit)
{
  return getLastWatchingClause(lit.getVariableNumber());
}

/*
 * Function: LiteralWatcherDatabase::addWatcher
 * Purpose:
 * Add a clause to the watcher list for some variable.
 * 
 */
void LiteralWatcherDatabase::addWatcher(Literal &l, Clause *c)
{
  mLiteralWatchers[l.getVariableNumber()].push_front(c);
  return;
}

/*
 * Function: LiteralWatcherDatabase:;removeWatcher
 * Purpose:
 * Remove a clause from the watcher list.
 */
void LiteralWatcherDatabase::removeWatcher(Literal &l, Clause *c)
{
  mLiteralWatchers[l.getVariableNumber()].remove(c);
  return;
}
