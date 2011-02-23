/*
 * 
 */

#ifndef _GALSAT_ADAPTIVE_GA_SOLVER_H_
#define _GALSAT_ADAPTIVE_GA_SOLVER_H_

#include <ga/ga.h>

#include "SimpleGASolver.h"

class AdaptiveGASolver : public SimpleGASolver
{
public:
  AdaptiveGASolver();
  
  virtual void beginAttemptSolve();
};

#endif
