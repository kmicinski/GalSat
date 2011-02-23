/*
 * File: DimacsParser.cc
 * Purpose: Implementation of the DIMACS CNF input file parser
 * Author: Kristopher Micinski
 * Date: November 1st, 2010
 */

#include "DimacsParser.h"
#include "Literal.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>

using namespace std;

/*
 * Name: DimacsParser::attemptParseFile
 * Purpose:
 * Attempt to parse a file in the standard DIMACS format and
 * produce a new problem from it.
 * Arguments:
 * None, but the filename should have been setup previously.
 */
void DimacsParser::attemptParseFile()
{
  bool haveSeenProblem = false;
  string inputLine;
  istringstream inStream;
  unsigned long long variables;
  unsigned long long clauses;
  
  mParsedFile = false;
  
  // Check to see if the user has allocated anything
  if (mProblem) 
    {
      cerr << "Solver did not claim problem!\n";
    }
  
  ifstream inputFile (mFilename.c_str());
  
  if (inputFile.is_open())
    {
      while (!inputFile.eof())
	{
	  getline (inputFile,inputLine);
	  
	  // Process the line
	  if (!haveSeenProblem)
	    {
	      char firstChar;
	      string str;
	      
	      inStream.str(inputLine);
	      inStream >> firstChar;
	      
	      switch (firstChar)
		{
		case 'c':
		  continue;
		case 'p':
		  inStream >> str;
		  inStream >> variables;
		  inStream >> clauses;
		  
		  if (str == "cnf")
		    {
		      mProblem = new Problem(mClauseAllocator);
		      mProblem->initProblem(variables);
		    }
		  
		  haveSeenProblem = true;
		}
	    }
	  else
	    {
	      // We are on a standard, (i.e., "clause,") line.
	      list<Literal> problemLiterals;
	      
	      istringstream lineString(inputLine);
	      
	      if (inputLine == "")
		continue;
	      
	      while(lineString.good())
		{
		  Literal l;
		  lineString >> l;
		  // Hack! for end of line...
		  if (l.getVariableNumber() == 0)
		    continue;
		  l.setVariableNumber(l.getVariableNumber()-1);
		  problemLiterals.push_front(l);
		}
	      
	      Clause *newClause = mClauseAllocator->requestNewClause(problemLiterals.size());
	      
	      *newClause = problemLiterals;
	      //cout << endl << *newClause << endl;
	      mProblem->attachClause(newClause);
	    }
	  
	  if (mProblem->getNumClauses() >= clauses)
	    {
	      // If we have successfully parsed enough clauses
	      mParsedFile = true;
	      return;
	    }
	}
    }
  return;
}

Problem *DimacsParser::extractParsedProblem()
{
  if (mProblem == NULL || !mParsedFile)
    {
      return NULL;
    }
  else
    {
      Problem *ret = mProblem;
      mProblem = NULL;
      return ret;
    }
}

