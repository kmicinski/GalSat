/*
 * File: PenaltyTracker.h
 * Purpose:
 * We implement a penalty function on clauses in SAT. The idea here is to push *harder* against clauses
 * which have been conflicting more recently. This is somewhat like VSIDS or other adaptive variable 
 * selection algorithms.
 * 
 * To facalitate the algorithm, we use this penalty tracker class. The class is a container which 
 * gives the penalty for some clause as a real number in the range [0..1].
 * 
 * For example, if the clause database contains clauses:
 * C1 -- [ 0, 1, -2 ]
 * C2 -- [ 1, -2, -3 ]
 * C3 -- [ 1, 2 ]
 * C4 -- [ 2, -3 ]
 * 
 * Then if we have a number of variable assignments to this clause database:
 * [ 0, 1, 1, 0 ]
 * [ 1, 0, 1, 0 ]
 * [ 0, 0, 1, 1 ]
 * 
 */

#ifndef _GALSAT_PENALTY_TRACKER_H_
#define _GALSAT_PENALTY_TRACKER_H_

#include <vector>

using namespace std;

class PenaltyTracker
{
private:
  // The actual (unscaled) clause scores over time...
  vector<double> mClauseScores;
  
  // The table of the number of clauses hit in the current generation...
  vector<unsigned> mClauseHitsTable;
  
  // The total weight of the clause table...
  double mTotalClauseScore;
  
public:
  PenaltyTracker(unsigned long long numClauses);
  
  // Take the clause weights from the current generation and merge them with 
  // the previous generations...
  virtual void moveToNewGeneration();
  
  // Inform the penalty tracker that a clause in the current generation has been 
  // found conflicting...
  virtual void bumpClauseInCurrentGeneration(unsigned long long clauseNumber);
  
  // Get the current (normalized) penalty weight for a single clause violation
  // from the data of the last generation. This does not incorporate data from
  // the current generation unless you explicitly call moveToNewGeneration()
  // beforehand.
  virtual double getCurrentNormalizedPenalty(unsigned long long clauseNumber);
};

#endif
