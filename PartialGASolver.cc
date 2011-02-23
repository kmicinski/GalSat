/*
 * File: PartialGASolver.cc
 * 
 * Author: Kristopher Micinski
 * Date: December 4th, 2010
 */

#include "PartialGASolver.h"
#include "GAVariableAssignmentGenome.h"
#include "Problem.h"

#include <ga/ga.h>

/*
 * PartialGASolver::PartialGASolver()
 * Purpose:
 * Set up basic information to use the partial assignment 
 * based technique.
 */
PartialGASolver::PartialGASolver()
{
  mSolverInfo.mSolverName = "partialgasolver";
  mSolverInfo.mSolverDescription = "A GA based solver using the partial assignment technique";
  mSolverInfo.mSolver = this;
  
  return;
}

/*
 * Function: PartialGASolver::beginAttemptSolve()
 * Purpose:
 * Set up and use the Partial GA technique.
 */
void PartialGASolver::beginAttemptSolve()
{
  mPopulationSize = 100;
  mNumGenerations = 1000;
  mMutationRate = 1.0/(mProblem->getNumVariables());
  mCrossoverRate = 0.85;
  
  cout << "Searching for satisfying assignment using partial GA Assignment technique...\n";
  
  GAPartialVariableAssignmentGenome sampleGenome(mProblem);
  
  GASimpleGA ga(sampleGenome);
  ga.nGenerations(mNumGenerations);
  ga.flushFrequency(10);
  ga.terminator(PartialGASolver::GATerminateUponSuccess);
  
  // Do the evolution...
  ga.evolve();
  
  cout << ga.statistics() << endl;
  
  const GAStatistics &statistics = ga.statistics();
  
  mWorkingAssignment = new VariableAssignment(mProblem->getNumVariables());
  
  const GAVariableAssignmentGenome &genome = ((GAVariableAssignmentGenome &)statistics.bestIndividual());
  
  for (unsigned int i = 0; i < mProblem->getNumVariables(); i++)
    {
      if (genome.gene(2*i+1) == 0)
	{
	  mWorkingAssignment->assignLiteral(i,Literal::POLARITY_UNASSIGNED);
	}
      else
	{
	  // Variable assigned...
	  if (genome.gene(2*i) == 0) 
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
  
  mModel = mWorkingAssignment;
  
  cout << *mWorkingAssignment;

  cout << endl << endl << mConflictingClauses.size() << " conflicting clauses...\n\n";

  return;
}
