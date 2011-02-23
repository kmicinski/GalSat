/* 
 * File: VariableAssignment.h
 * Purpose: Present a defined mapping between variables and polarities for some problem.
 * 
 * Author: Kristoper Micinski
 * Date: November 7th, 2010.
 */

#ifndef _GALSAT_VARIABLE_ASSIGNMENT_H_
#define _GALSAT_VARIABLE_ASSIGNMENT_H_

#include <list>
#include <map>
#include <ostream>

#include "Problem.h"
#include "Literal.h"
//#include "VariableAssignmentWatcher.h"

class VariableAssignment
{
private:
  //list<VariableAssignmentWatcher *> mAssignmentWatcher;
  vector<Literal> mAssignedLiterals;
  
  const unsigned long mNumVariables;
  unsigned long mNumAssignedVariables;
  
  friend ostream &operator<<(ostream &, const VariableAssignment&);
public:
  VariableAssignment(unsigned long numVariables);
  virtual ~VariableAssignment();
  
  //  void addAssignmentWatcher(VariableAssignmentWatcher *watcher);
  //void removeAssignmentWatcher(VariableAssignmentWatcher *watcher);
  
  unsigned long getNumAssignedVariables() { return mNumAssignedVariables; }
  
  // Assign a variable to a certain literal
  virtual void assignLiteral(Literal &l);
  virtual void assignLiteral(unsigned long varNumber, Literal::Polarity polarity);
  
  // Get the polarity of a certain variable
  Literal::Polarity queryVariablePolarity(unsigned long variableNumber)
    { return mAssignedLiterals[variableNumber].getPolarity(); }
  
  Literal queryVariable(unsigned long variableNumber)
  { return mAssignedLiterals[variableNumber]; }
  
  //Literal::Polarity operator[](unsigned  long i) { return queryVariablePolarity(i); }
  
};

#if 0
/*
 * Class: NumAssignedTrackingVariableAssignment
 * Purpose:
 * The VariableAssignment class keeps track of variable assignments, but it doesn't 
 * include methods to keep track of how *many* variables are assigned at one time. 
 * I.e., if you want to know how many variables are assigned in some assignment, you
 * have to go through the assignment each time. This is an O(n) operation for keeping 
 * track of something that should otherwise be fairly simple to do. Instead, 
 * this derived class keeps track of the number of assigned variables, however it does 
 * so with a slight increased time complexity of each call to assignLiteral().
 * 
 * If you don't need to use the number of variables in an assignment, it's prefered that 
 * you use VariableAssignment because it has lower time on average, but if you are 
 * going to frequently be needing access to the number of assigned variables (for 
 * example, in an evaluation function...) then this class will provide you with lower 
 * amortized time.
 */
class NumAssignedTrackingVariableAssignment : public VariableAssignment
{
private:
  unsigned long numAssignedVariables;
  
public:
  NumAssignedTrackingVariableAssignment(unsigned long numVariables);
  virtual ~NumAssignedTrackingVariableAssignment();
  
  unsigned long getNumAssignedVariables() { return numAssignedVariables; }
  
  virtual void assignLiteral(unsigned long varNumber, Literal::Polarity polarity);
  virtual void assignLiteral(Literal &l);
}

#endif

#endif

