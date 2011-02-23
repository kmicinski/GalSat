/*
 * File: GAVariableAssignmentGenome.cc
 * Purpose:
 * Implementation of the variable assignment genome.
 */

#include "GAVariableAssignmentGenome.h"

#include <iostream>
#include <ga/ga.h>

using namespace std;

GAVariableAssignmentGenome::GAVariableAssignmentGenome(Problem *problem)
  : GA1DBinaryStringGenome((unsigned int)problem->getNumVariables())
{
  evaluator(staticevaluate);
  setProblem(problem);
  
  return;
}

void GAVariableAssignmentGenome::copy(const GAGenome &c)
{
  GAVariableAssignmentGenome &copy = (GAVariableAssignmentGenome &)c;

  // Do the super function...
  GA1DBinaryStringGenome::copy(c);
  
  // simply copy the problem, we don't need anything else...
  copy.setProblem(mProblem);
  
  return;
}

/*
 * GAPartialVariableAssignmentGenome::currentFitness()
 * Purpose:
 * The evaluation function for the partial assignment genome.
 */
float GAPartialVariableAssignmentGenome::currentFitness()
{
  float satisfied_fitness = 
    ((float)mSatisfiedClauses.size())/mProblem->getNumClauses();
  
  float variable_assignment_fitness =
    ((float)mWorkingAssignment->getNumAssignedVariables())/mProblem->getNumVariables();
  
  //  ((float)
  float result = 
    satisfied_fitness * 1; //variable_assignment_fitness;
  return result;
}

/*
 * 
 */
float GAPartialVariableAssignmentGenome::evaluate(GABoolean flag)
{
  float result = 0.0;
  mWorkingAssignment = new VariableAssignment(mProblem->getNumVariables());
  unsigned long unassigned = 0;
  unsigned long assigned = 0;

  for (unsigned int i = 0; i < mProblem->getNumVariables(); i++)
    {
      if (gene(2*i+1) == 0)
	{
	  unassigned++;
	  mWorkingAssignment->assignLiteral(i,Literal::POLARITY_UNASSIGNED);
	}
      else
	{
	  assigned++;
	  // Variable assigned...
	  if (gene(2*i) == 0) 
	    {
	      // False variable
	      mWorkingAssignment->assignLiteral(i,Literal::POLARITY_FALSE);
	    }
	  else
	    {
	      mWorkingAssignment->assignLiteral(i,Literal::POLARITY_TRUE);
	    }
	}
    }
  
  recalculateConflictingClauses();
  
  // Now run GSAT-Partial
  mMaxFlips = 50;
  
  while (!evaluateComplete())
    {
      if (--mMaxFlips == 0)
	{
	  break;
	}
      // Climb a little...
      stepOnce();
    }
  
  float fitness = currentFitness();
  mConflictingClauses.clear();
  delete mWorkingAssignment;

  return fitness;
}

/*
 * Function: GAPartialVariableAssignmentGenome::assignAndCalculateConflicts
 * Purpose:
 * This is a function in GSatPartialSolver, and used to assign variables to the variable 
 * assignment set. However, the stepOnce() function uses it, and we want to intercept 
 * the calls from that function so that when we change a variable in the variable 
 * database, we also change the assignmet in the gene.
 */
void GAPartialVariableAssignmentGenome::assignAndCalculateConflicts(unsigned long variableNumber, Literal::Polarity polarity)
{
  GSatPartialSolver::assignAndCalculateConflicts(variableNumber, polarity);
  
  // Now change it in the genome...
  switch (polarity)
    {
    case Literal::POLARITY_UNASSIGNED:
      gene(variableNumber*2+1,0); // Set to unassigned
      break;
    case Literal::POLARITY_FALSE:
      gene(variableNumber*2+1,1); // Assign!
      gene(variableNumber*2,0);   // False!
      break;
    case Literal::POLARITY_TRUE:
      gene(variableNumber*2+1,1); // Assign!
      gene(variableNumber*2,1);   // True!
      break;
    }
  
  return;
}

/*
 * Function: GAVariableAssignmentGenome::flipAndCalculateConflicts
 * Purpose:
 * This is a function in GSatSolver, and used to assign variables to the variable 
 * assignment set. However, the stepOnce() function uses it, and we want to intercept 
 * the calls from that function so that when we change a variable in the variable 
 * database, we also change the assignmet in the gene.
 */
void GAVariableAssignmentGenome::flipAndCalculateConflicts(unsigned long variableNumber)
{
  GSatSolver::flipAndCalculateConflicts(variableNumber);
  
  if (gene(variableNumber) == 1)
    {
      gene(variableNumber,0);
    }
  else
    {
      gene(variableNumber,1);
    }
  
  return;
}

void GAPartialVariableAssignmentGenome::copy(const GAGenome &c)
{
  GAPartialVariableAssignmentGenome &copy = (GAPartialVariableAssignmentGenome &)c;

  // Do the super function...
  GA1DBinaryStringGenome::copy(c);
  
  // simply copy the problem, we don't need anything else...
  copy.setProblem(mProblem);
  
  return;
}

GAPartialVariableAssignmentGenome::GAPartialVariableAssignmentGenome(Problem *problem)
  : GA1DBinaryStringGenome((unsigned int)problem->getNumVariables() * 2)
{
  evaluator(staticevaluate);
  setProblem(problem);
  
  return;
}

float GAVariableAssignmentGenome::evaluate(GABoolean flag)
{
  float result = 0.0;
  mWorkingAssignment = new VariableAssignment(mProblem->getNumVariables());
  
  for (unsigned int i = 0; i < mProblem->getNumVariables(); i++)
    {
      if (gene(i) == 0) 
	{
	  // False variable
	  mWorkingAssignment->assignLiteral(i,Literal::POLARITY_FALSE);
	}
      else
	{
	  mWorkingAssignment->assignLiteral(i,Literal::POLARITY_TRUE);
	}
    }
  
  recalculateConflictingClauses();
  

  // Set a conservative number of max flips
  mMaxFlips = 50;
  
  while (!evaluateComplete()) {
    if (--mMaxFlips == 0)
      {
	break;
      }
    
    stepOnce();
  }
  
  result = (1.0 - (((float)getNumConflictingClauses())/getNumClauses()));
  
  mConflictingClauses.clear();
  delete mWorkingAssignment;
  
  //  cout << "hello.";
  return result;
}
