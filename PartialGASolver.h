/*
 * File: PartialGASolver.h
 * 
 * Author: Kristopher Micinski
 * Date: December 4th, 2010
 */

#ifndef _GALSAT_PARTIAL_GASOLVER_H_
#define _GALSAT_PARTIAL_GASOLVER_H_

#include "SimpleGASolver.h"

class PartialGASolver : public SimpleGASolver
{
private:
  
public:
  PartialGASolver();
  
  virtual void beginAttemptSolve();

  static float Objective(GAGenome &g);
    
};

#endif
