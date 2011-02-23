/*
 *
 */

#include <fstream>

#include "Literal.h"
#include "Clause.h"
#include "Problem.h"

/*
 * Function: Clause::Clause
 * Purpose:
 * 
 */
Clause::Clause (unsigned long numVariables, unsigned long long clauseNumber)
{
  mNumLiterals = numVariables;
  mClauseNumber = clauseNumber;
}

/*
 *
 */
Clause& Clause::operator=(Clause &clause)
{
  return *this;
}
  
/*
 *
 */
Clause& Clause::operator=(list<Literal> &literals)
{
  mNumLiterals = literals.size();
  for (list<Literal>::iterator lit = literals.begin(); lit != literals.end(); lit++)
    {
      Literal literal = *lit;
      mLiterals.push_back(literal);
    }
  return *this;
}

/*
 *
 */
void Clause::addLiteral(Literal l)
{
  mLiterals.push_back(l);
  mNumLiterals++;
  
  return;
}

/*
 *
 */
void Clause::setLiteral(unsigned long literalNumber, Literal::Polarity polarity)
{
  mLiterals[literalNumber].setPolarity(polarity);
}  

/*
 * 
 */
Literal const Clause::getLiteral(unsigned long literalNumber)
{
  return mLiterals[literalNumber];
}

/*
 * Function: Clause::removeFromWatcherLists()
 * Purpose:
 * We eventually need to remove a clause from its watchers watched lists...
 * we do this using this function.
 */
void Clause::removeFromWatcherLists()
{
  for (LiteralIterator lit = mLiterals.begin(); lit != mLiterals.end(); lit++)
    {
      mProblem->getLiteralWatcherDB()->removeWatcher(*lit,this);
    }
  return;
}

/*
 * Function: Clause::clearClause
 * Purpose:
 * clean up a clause from the database.
 */
void Clause::clearClause()
{
  removeFromWatcherLists();
  mLiterals.clear();
}

ostream& operator<<(ostream &stream,Clause &c)
{
  stream << "[ ";
  
  for (Clause::LiteralIterator i = c.mLiterals.begin(); i != c.mLiterals.end(); i++)
    {
      Literal l = *i;
      stream << l;
      stream << ", ";
    }
  
  stream << "]";
  
  return stream;
}
