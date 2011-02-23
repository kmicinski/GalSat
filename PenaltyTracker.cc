/*
 * File: PenaltyTracker.cc
 * Purpose:
 * Implementation of penalty tracker.
 */

#include "PenaltyTracker.h"

/*
 * Function: PenaltyTracker::PenaltyTracker
 * Purpose:
 * Construct and initialize the set of penalty weights.
 */
PenaltyTracker::PenaltyTracker(unsigned long long numClauses)
{
  mClauseScores.resize(numClauses);
  
  // Initially assume that all clauses will have the same probability 
  // of being violated...
  for (unsigned long clause = 0; clause < numClauses; clause++)
    {
      mClauseScores[clause] = 1.0;
    }
  
  mTotalClauseScore = numClauses;
  
  mClauseHitsTable.resize(numClauses);
  
  for (unsigned long clause = 0; clause < numClauses; clause++)
    {
      mClauseHitsTable[0];
    }
}

/*
 * Function: PenaltyTracker::bumpCluaseInCurrentGeneration
 * Purpose:
 * Inform the current generation of the penalty tracker
 * clauses that some clause was violated.
 */
void PenaltyTracker::bumpClauseInCurrentGeneration(unsigned long long clauseNumber)
{
  ++mClauseHitsTable[clauseNumber];
  return;
}

/*
 * Function: PenaltyTracker::getCurrentNormalizedPenalty
 * Purpose:
 * Get the current normalized penalty of some clause in the 
 * penalty watcher database.
 */
double PenaltyTracker::getCurrentNormalizedPenalty(unsigned long long clauseNumber)
{
  return mClauseScores[clauseNumber]/mTotalClauseScore;
}

/*
 * Function: PenaltyTracker::moveToNewGeneration
 * Purpose:
 * Migrate the current set of clause penalities into the current generation...
 */
void PenaltyTracker::moveToNewGeneration()
{
  double newTotalClauseScore = 0;
  
  for (unsigned long clause = 0; clause < mClauseScores.size(); clause++)
    {
      mClauseScores[clause] =
	mClauseScores[clause]/2 + (mClauseHitsTable[clause]);
      newTotalClauseScore += mClauseScores[clause];
    }
  
  for (unsigned long clause = 0; clause < mClauseScores.size(); clause++)
    {
      mClauseHitsTable[clause] = 0;
    }
  return;
}
