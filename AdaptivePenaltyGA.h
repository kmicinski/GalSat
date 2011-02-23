/*
 * File: AdaptivePenaltyGA.h
 * Purpose:
 * Implementation of the adaptive penalty Genetic Algorithm...
 */

#ifndef _GALSAT_ADAPTIVE_PENALTY_GA_
#define _GALSAT_ADAPTIVE_PENALTY_GA_

#include <ga/ga.h>

#include "PenaltyTracker.h"
#include "Problem.h"

class AdaptivePenaltyGA : public GASimpleGA
{
private:
  PenaltyTracker *mPenaltyTracker;
  
public:
  GADefineIdentity("AdaptivePenaltyGA", 203);
  
  AdaptivePenaltyGA(const GAGenome& g,Problem *prob) : GASimpleGA(g) 
  { }
  AdaptivePenaltyGA(const GAPopulation& p, Problem *prob) : GASimpleGA(p)
  { }
  AdaptivePenaltyGA(const GASimpleGA& ga, Problem *prob) : GASimpleGA(ga) 
  { }
  
  PenaltyTracker *getPenaltyTracker() { return mPenaltyTracker; }
  void setPenaltyTracker(PenaltyTracker *tracker) { mPenaltyTracker = tracker; }
  
  // The main stepping difference...
  virtual void step();
};

#endif
