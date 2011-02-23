/*
 *
 */

#include <set>

#include "AdaptivePenaltyGenome.h"
#include "Problem.h"

using namespace std;

AdaptivePenaltyGenome::AdaptivePenaltyGenome(Problem *mProblem)
  : GAVariableAssignmentGenome(mProblem)
{
  evaluator(staticevaluate);
  setProblem(mProblem);
  
  return;
}

void AdaptivePenaltyGenome::copy(const GAGenome &c)
{
  AdaptivePenaltyGenome &copy = (AdaptivePenaltyGenome &)c;
  
  // Do the super...
  GAVariableAssignmentGenome::copy(c);
  
  copy.setPenaltyTracker(mPenaltyTracker);
  
  return;
}

float AdaptivePenaltyGenome::evaluate(GABoolean flag)
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
  
  float penalty = 0.0;
  
  // Compute penalty, and bump for next time...
  for (std::set<Problem::ProblemClausePtr>::iterator i = mConflictingClauses.begin(); i != mConflictingClauses.end();
       i++)
    {
      //
      Clause *c = *i;
      penalty += mPenaltyTracker->getCurrentNormalizedPenalty(c->getClauseNumber());
      mPenaltyTracker->bumpClauseInCurrentGeneration(c->getClauseNumber());
    }
  
  mConflictingClauses.clear();
  delete mWorkingAssignment;
  
  //  cout << "hello.";
  return result * (1.0 - 0.3*penalty);
}

