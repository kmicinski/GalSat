/*
 *
 */

#include "AdaptiveGASolver.h"
#include "PenaltyTracker.h"
#include "AdaptivePenaltyGenome.h"
#include "AdaptivePenaltyGA.h"

/*
 * Function: AdaptiveGASolver::AdaptiveGASolver()
 * Purpose:
 * Set up information for the adaptive solver.
 */
AdaptiveGASolver::AdaptiveGASolver()
{
  mSolverInfo.mSolverName = "adaptgasolver";
  mSolverInfo.mSolverDescription= "A GA based solver using Kris' adaptive penalty function";
  mSolverInfo.mSolver = this;
  
  return;
}

/*
 * Function: AdaptiveGASolver::beginAttemptSolve()
 * Purpose:
 * Solve using adaptive method.
 */
void AdaptiveGASolver::beginAttemptSolve()
{
  mPopulationSize = 100;
  mNumGenerations = 1000;
  mMutationRate = 1.0/(mProblem->getNumVariables());
  mCrossoverRate = 0.85;
  
  //float (*mObjectiveFunction)(GAGenome &) = ;
  
  PenaltyTracker *tracker = new PenaltyTracker(mProblem->getNumClauses());
  
  AdaptivePenaltyGenome sampleGenome(mProblem);
  sampleGenome.setPenaltyTracker(tracker);
  
  AdaptivePenaltyGA ga(sampleGenome, mProblem);
  ga.setPenaltyTracker(tracker);
  
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
