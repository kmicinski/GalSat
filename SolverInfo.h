/*
 */

#ifndef _GALSAT_SOLVER_INFO_H_
#define _GALSAT_SOLVER_INFO_H_

#include <string>

using namespace std;

class SatSolver;

struct SolverInfo
{
  string mSolverName;
  string mSolverDescription;
  SatSolver *mSolver;
};

#endif
