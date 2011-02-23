/*
 * File: AdaptivePenaltyGA.cc
 * Purpose:
 * Implementation of the adaptive penalty genetic algorithm strategy.
 */

#include "AdaptivePenaltyGA.h"

/*
 * Function: AdaptivePenaltyGA::step()
 * Purpose:
 * step the genetic algorithm one step further.
 */
void AdaptivePenaltyGA::step()
{
  GASimpleGA::step();
  mPenaltyTracker->moveToNewGeneration();
  return;
}
