/*
 * File: galsat.cpp
 * Purpose:
 * Main implementation of the GalSat prover.
 * 
 * Author: Kristopher Micinski
 * Date: November 1st, 2010.
 */

#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>

#include "SatSolver.h"
#include "EnumerativeSolver.h"
#include "DimacsParser.h"
#include "ClauseAllocator.h"
#include "GSatSolver.h"
#include "GSatPartialSolver.h"
#include "SimpleGASolver.h"
#include "PartialGASolver.h"
#include "AdaptiveGASolver.h"
#include "util.h"

using namespace std;

// A map of our solvers.
map<string, SolverInfo *> solversMap;

bool galsatVerbose;

EnumerativeSolver enumSolver;
GSatSolver gSatSolver;
GSatPartialSolver gSatPartialSolver;
SimpleGASolver simpleGASolver;
AdaptiveGASolver adaptiveGASolver;
PartialGASolver partialGASolver;
ClauseAllocator clauseAllocator;
Problem *toplevelProblem;
DimacsParser *dimacsParser;

char *inputFile;

// Environment settings
struct Environment Environment;

void init_main_options ()
{ 
  return;
}

void run_provers()
{

  if (inputFile == NULL)
    {
      cerr << "No input file specified!\n";
      exit(1);
    }
  
  cout << "Parsing input file... ";
  
  dimacsParser = new DimacsParser(&clauseAllocator, inputFile);
  dimacsParser->attemptParseFile();
  
  toplevelProblem = dimacsParser->extractParsedProblem();
  
  if (toplevelProblem == NULL)
    {
      cerr << "Error! Could not parse input file!\n";
      exit(1);
    }
    
  cout << "Done!\n";
  
  if (Environment.verbose)
    {
      
      cout << "Input clauses:" << endl;
  
      for (Problem::ProblemClausePtrIterator iter = toplevelProblem->getProblemClauses();
	   iter != toplevelProblem->getProblemClausesEnd(); iter++)
	{
	  cout << **iter << endl;
	}
    }
  
  
  SolverInfo *info = solversMap[Environment.solver];
  
  if (!info) 
    {
      cerr << "No or incorrect solver specified..." << endl;
      exit(1);
    }
  
  cout << "Running solver...\n";
  SatSolver *solver = info->mSolver;
  
  solver->setProblem(toplevelProblem);
  
  // Run solvers...
  solver->beginAttemptSolve();
  
  if (solver->getModelFound())
    {
      if (solver->checkModel())
	{
	  // Yea! We found a model!
	  cout << "\n\nFound model! Printing now...\n\n";
	  solver->printModel();
	}
      else
	{
	  cout << "\nModel is incorrect!\n";
	}
    }
  else
    {
      cout << "No model found!\n";
    }
  
  
  return;
}

void init_solvers()
{
  cout << "Loading solvers...\n";
  
  clauseAllocator.setMaxClauses(100000);
  
  solversMap[enumSolver.getSolverInfo()->mSolverName] = enumSolver.getSolverInfo();
  solversMap[gSatSolver.getSolverInfo()->mSolverName] = gSatSolver.getSolverInfo();
  solversMap[simpleGASolver.getSolverInfo()->mSolverName] = simpleGASolver.getSolverInfo();
  solversMap[partialGASolver.getSolverInfo()->mSolverName] = partialGASolver.getSolverInfo();
  solversMap[gSatPartialSolver.getSolverInfo()->mSolverName] = gSatPartialSolver.getSolverInfo();
  solversMap[adaptiveGASolver.getSolverInfo()->mSolverName] = adaptiveGASolver.getSolverInfo();
  
  return;
}

int main (int argc, char **argv)
{
  int c;
  inputFile = NULL;
  
  cout << "The GalSat Prover -- By Kristopher Micinski\n\n";

  init_solvers();
  
  while ((c = getopt (argc, argv, "hi:s:vf:")) != -1)
    {
      switch (c)
	{
	case 'h':
	  cout << "Options:\n\n";
	  cout << "\t-h -- Print this help screen\n";
	  cout << "\t-s solver -- Instruct GalSat to user a certain solver\n";
	  cout << "\t-i inputfile -- Provide an input file in DIMACS format.\n";
	  cout << "\nAvailable solvers:\n";
	  for (map<string, SolverInfo *>::const_iterator i = solversMap.begin();
	       i != solversMap.end(); i++)
	    {
	      pair<string,SolverInfo *> it = *i;
	      SolverInfo *solverInfo = it.second;
	      cout << "\t" << solverInfo->mSolverName.c_str() << "\t-- " << solverInfo->mSolverDescription.c_str() << "\n";
	    }
	  exit(0);
	  break;
	case 'i':
	  inputFile = optarg;
	  break;
	case 'v':
	  Environment.verbose = true;
	  break;
	case 'f':
	  //Environment.numflips = 
	case 's':
	  Environment.solver = optarg;
	  cout << (Environment.solver) <<endl;
	  break;
	}
    }


  run_provers();
  
  return 0;
}

