/*
 * File: Literal.cc
 * Purpose:
 * Implement literals and their associted operations
 * 
 * Creation: November 1st, 2010.
 * Author: Kristopher Micinski
 */

#include <iostream>

using namespace std;

#include "Literal.h"

/*
 * Function: Literal::operator>>
 * Purpose:
 * Input operator for literal
 * Arguments:
 */
istream &operator>>(istream &input, Literal &l)
{
  long inputNum;
  
  if (!input.good())
    {
      // Can't read anything!
      return input;
    }
  
  input >> inputNum;
  
  if (inputNum < 0) 
    {
      l.mPolarity = Literal::POLARITY_FALSE;
      l.mVariableNumber = -inputNum;
    }
  else
    {
      l.mPolarity = Literal::POLARITY_TRUE;
      l.mVariableNumber = inputNum;
    }
  
  return input;
}

ostream &operator<<(ostream &output, Literal l)
{
  switch (l.getPolarity())
    {
    case Literal::POLARITY_TRUE:
      output << l.getVariableNumber();
      break;
    case Literal::POLARITY_FALSE:
      output << "-" << l.getVariableNumber();
      break;
    case Literal::POLARITY_UNASSIGNED:
      output << "" << l.getVariableNumber() << "(unassigned)";
      break;
    }
  return output;
}
