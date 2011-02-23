/*
 * 
 */

#ifndef _GALSAT_ADAPTIVE_PENALTY_GENOME_H_
#define _GALSAT_ADAPTIVE_PENALTY_GENOME_H_

#include "GAVariableAssignmentGenome.h"
#include "PenaltyTracker.h"

class AdaptivePenaltyGenome : public GAVariableAssignmentGenome
{
private:
  PenaltyTracker *mPenaltyTracker;
  
protected:
  //virtual void flipAndCalculateConflicts(unsigned long variableNumber);
  
public:
  GADefineIdentity("GAVariableAssignmentGenome", 205);
  
  AdaptivePenaltyGenome(Problem *problem);
  AdaptivePenaltyGenome(const GAGenome& g) :GAVariableAssignmentGenome::GAVariableAssignmentGenome((GAVariableAssignmentGenome &)g) {  copy(g); }
  
  virtual GAGenome *clone(CloneMethod) const { return new AdaptivePenaltyGenome(*this); }
  
  // The copy function to copy this genome from another genome
  virtual void copy(const GAGenome &c);
  
  // The evaluation function
  virtual float evaluate(GABoolean flag = gaFalse);
    
  static float staticevaluate(GAGenome &g) { return ((AdaptivePenaltyGenome &)g).evaluate(); }
  
  PenaltyTracker *getPenaltyTracker() { return mPenaltyTracker; }
  void setPenaltyTracker(PenaltyTracker *p) { mPenaltyTracker = p; }
  
};

#endif
