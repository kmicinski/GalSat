/*
 * File: SimpleGASolver.h
 * 
 * Author: Kristopher Micinski
 * Date: November 19th, 2010
 */

#ifndef _GALSAT_SIMPLE_GASOLVER_H_
#define _GALSAT_SIMPLE_GASOLVER_H_

#include <ga/ga.h>

#include "GSatSolver.h"

class SimpleGASolver : public GSatSolver
{
protected:
  int mPopulationSize;
  int mNumGenerations;
  float mMutationRate;
  float mCrossoverRate;
  
public:
  SimpleGASolver();
  
  static float Objective(GAGenome &g);
  
  static GABoolean GATerminateUponSuccess(GAGeneticAlgorithm &ga);
  
  virtual void beginAttemptSolve();
};

#endif
