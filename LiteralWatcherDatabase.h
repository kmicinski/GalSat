/*
 * File: LiteralWatcherDatabase.h
 * Purpose:
 * Describe a set of watched literals and their associated watchers.
 *
 * Author: Kristopher Micinski
 * Date: November 1st, 2010
 */

#ifndef _GALSAT_LITERAL_WATCHERDB_H_
#define _GALSAT_LITERAL_WATCHERDB_H_

#include <list>
#include <vector>

using namespace std;

#include "Literal.h"

class Clause;

class LiteralWatcherDatabase
{
private:  
  vector<list<Clause *> > mLiteralWatchers;
    
public:
  typedef list<Clause *> ClauseWatcherList;
  
  virtual ~LiteralWatcherDatabase();
  
  LiteralWatcherDatabase(unsigned long long NumVariables);
  
  unsigned long long getNumLiterals() { return mLiteralWatchers.size(); }
  
  // Add a clause to the watcher list
  virtual void addWatcher(Literal &l, Clause *);
  
  // Remove a clause from a watcher list
  virtual void removeWatcher(Literal &l, Clause *);
  
  // Get the list of clauses watching this variable
  list<Clause *>::iterator getWatchingClauses(unsigned long variable);
  
  // Same as previous but with a literal to indicate which variable.
  list<Clause *>::iterator getWatchingClauses(Literal &lit);
  
  list<Clause *>::iterator getLastWatchingClause(unsigned long variable);
  
  list<Clause *>::iterator getLastWatchingClause(Literal &lit);
};

#endif
