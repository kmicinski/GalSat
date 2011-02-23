/*
 * File: SimpleGASolver.cc
 * 
 * Author: Kristopher Micinski
 * Date: November 19th, 2010
 */

#include "SimpleGASolver.h"
#include "GAVariableAssignmentGenome.h"
#include "Problem.h"

#include <ga/ga.h>

/*
 * Function: SimpleGASolver::SimpleGASolver()
 * Purpose:
 * Set up some basic information about the GA based solver.
 */
SimpleGASolver::SimpleGASolver()
{
  mSolverInfo.mSolverName = "simplegasolver";
  mSolverInfo.mSolverDescription = "A simple GA based solver for SAT";
  mSolverInfo.mSolver = this;
  
  return;
}

/*
 * Function: SimpleGASolver::beginAttemptSolve()
 * Purpose:
 * 
 */
void SimpleGASolver::beginAttemptSolve()
{
  mPopulationSize = 100;
  mNumGenerations = 1000;
  mMutationRate = 1.0/(mProblem->getNumVariables());
  mCrossoverRate = 0.85;
  
  //float (*mObjectiveFunction)(GAGenome &) = ;

  GAVariableAssignmentGenome sampleGenome(mProblem);
  
  GASimpleGA ga(sampleGenome);
  ga.nGenerations(mNumGenerations);
  ga.flushFrequency(10);
  ga.terminator(SimpleGASolver::GATerminateUponSuccess);
  
  // Do the evolution
  ga.evolve();
  
  cout << ga.statistics() << endl;
  
  const GAStatistics &statistics = ga.statistics();
  
  mWorkingAssignment = new VariableAssignment(mProblem->getNumVariables());
  
  const GAVariableAssignmentGenome &genome = ((GAVariableAssignmentGenome &)statistics.bestIndividual());
  
  for (unsigned int i = 0; i < mProblem->getNumVariables(); i++) 
    {
      if (genome.gene(i) == 0)
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
  
  mModel = mWorkingAssignment;
  
  cout << *mWorkingAssignment;

  cout << endl << endl << mConflictingClauses.size() << " conflicting clauses...\n\n";
    
  return;
}

/*
 * Function: SimpleGASolver::isDone
 * Purpose:
 * Tell the Genetic Algorithm whether or not the genetic algorithm has 
 * completed or not.
 */
GABoolean SimpleGASolver::GATerminateUponSuccess(GAGeneticAlgorithm & ga){
  return ((ga.statistics().bestIndividual().score() == 1.0) ? gaTrue : gaFalse);
}
