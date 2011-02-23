/*
 * File: VariableAssignment.cc
 * Purpose:
 * Act as a container class for variable assignments that will be tested in GalSat.
 *
 * Author: Kristopher Micinski
 * Date: November 7th, 2010.
 */

#include "VariableAssignment.h"
#include <vector>

using namespace std;

/*
 * Function: VariableAssignment::VariableAssignment
 * Purpose:
 * Constructor for a variable assignment
 */
VariableAssignment::VariableAssignment(unsigned long numVariables)
  : mNumVariables(numVariables),
    mNumAssignedVariables(0)
{
  // Set the assigned number of variables
  mAssignedLiterals.resize(numVariables);
  
  int num = 0;

  for (vector<Literal>::iterator i = mAssignedLiterals.begin();
       i != mAssignedLiterals.end(); i++)
    {
      Literal &literal = *i;
      literal.setVariableNumber(num++);
      literal.setPolarity(Literal::POLARITY_UNASSIGNED);
    }
    
  return;
}

/*
 * Function: ~VariableAssignment::VariableAssignment
 * Purpose:
 * Destructor, simply clears list of of assigned literals
 */
VariableAssignment::~VariableAssignment()
{
  //mAssignmentWatcher.clear();
  mAssignedLiterals.clear();
}

#if 0
/*
 * Function: NumAssignedTrackingVariableAssignment::NumAssignedTrackingVariableAssignment
 * Purpose:
 */
NumAssignedTrackingVariableAssignment::NumAssignedTrackingVariableAssignment(unsigned long numVariables)
  : VariableAssignment(numVariables),
    numAssignedVariables(0)
{
  return;
}

NumAssignedTrackingVariableAssignment::NumAssignedTrackingVariableAssignment(unsigned long numVariables)
{
  
}

#endif

/*
 * Function: VariableAssignment::assignLiteral
 * Purpose:
 * Assign some variable a certain polarity.
 * Arguments:
 * varNumber -- The variable number.
 * polarity -- The polarity to assign to the variable.
 */
void VariableAssignment::assignLiteral(unsigned long varNumber, Literal::Polarity polarity)
{
  if (polarity == Literal::POLARITY_UNASSIGNED)
    {
      // Now unassigned
      if (mAssignedLiterals[varNumber].getPolarity() != Literal::POLARITY_UNASSIGNED)
	{
	  // Previously assigned, now unassigned
	  --mNumAssignedVariables;
	}
    }
  else
    {
      // Now assigned
      if (mAssignedLiterals[varNumber].getPolarity() == Literal::POLARITY_UNASSIGNED)
	{
	  // Previously unassigned, now assigned
	  ++mNumAssignedVariables;
	}
    }

  mAssignedLiterals[varNumber].setPolarity(polarity);
  return;
}

/*
 * Function: VariableAssignment::assignLiteral
 * Purpose:
 * Assign some variable a certain polarity.
 * Arguments:
 * The function is the same as assignLiteral(varNumber,polarity), but with an interface to the
 * literal.
 */
void VariableAssignment::assignLiteral(Literal &literal)
{
  assignLiteral(literal.getVariableNumber(),literal.getPolarity());
  return;
}

/*
 * Function: operator<<
 * Purpose:
 * Output a variable assignment.
 */
ostream& operator<<(ostream &stream,const VariableAssignment &m)
{
  unsigned long long curVar = 0;
  
  while(curVar < m.mNumVariables)
    {
      for (int i = 0; i < 5 && curVar < m.mNumVariables; i++)
	{
	  stream << "  Var" << curVar << " = ";
	  if (m.mAssignedLiterals[curVar].getPolarity() == Literal::POLARITY_UNASSIGNED)
	    {
	      stream << "(unassigned),";
	    }
	  else if (m.mAssignedLiterals[curVar].getPolarity() == Literal::POLARITY_FALSE)
	    {
	     stream << "F,";
	    }
	  else
	    {
	      stream << "T,";
	    }
	  curVar++;
	}
      stream << endl;
    }
  
  return stream;
}

