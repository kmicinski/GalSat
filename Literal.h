/* 
 * File: Literal.h
 * Implementation: Literal.cc
 * Purpose:
 * Implementation of basic functionality for Literals in GalSAT.
 */

#ifndef _GALSAT_LITERAL_H_
#define _GALSAT_LITERAL_H_

#include <fstream>

using namespace std;

/*
 * Class: Literal
 * Purpose:
 * Implement the very basic functionality associated with literals.
 *
 * See also:
 * Clause (Clause.h) -- Where literals are stored
 */
class Literal
{
public:
  // The possible polarities of variables in the clause.
  enum Polarity
    {
      POLARITY_TRUE,
      POLARITY_FALSE,
      POLARITY_UNASSIGNED
    };

private:
  // The variable number of the literal
  unsigned long mVariableNumber;
  
  // The polarity of this variable
  Polarity mPolarity;
  
public:
  // Overloaded I/O handlers
  friend istream &operator>>(istream &input,Literal &l);
  friend ostream &operator<<(ostream &out, Literal l);
  
  Literal() : mPolarity(POLARITY_UNASSIGNED) { }
  Literal(unsigned long num, Polarity polarity) { mVariableNumber = num; mPolarity = polarity; }
  
  unsigned long getVariableNumber() { return mVariableNumber; }
  void setVariableNumber(unsigned long varNumber) { mVariableNumber = varNumber; }
  
  Polarity getPolarity() const { return mPolarity; }
  void setPolarity(Polarity polarity) { mPolarity = polarity; }
  
  // Overloaded operators
  int operator==(Literal &b) { return (b.getVariableNumber() == mVariableNumber); }
};

#endif

