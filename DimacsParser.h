/*
 *
 */

#ifndef _GALSAT_DIMACS_PARSER_H_
#define _GALSAT_DIMACS_PARSER_H_

#include <string>

#include "ClauseAllocator.h"
#include "Problem.h"

using namespace std;

class DimacsParser {
private:
  // Have we parsed the file or not?
  bool mParsedFile;
  
  // The problem with which to work
  Problem *mProblem;
  
  // The filename to parse
  string mFilename;
  
  ClauseAllocator *mClauseAllocator;
  
public:
  DimacsParser(ClauseAllocator *allocator, string name) : mClauseAllocator(allocator), mFilename(name), mProblem(NULL) { }
  
  // Attempt to parse the file
  void attemptParseFile();
  
  // Get the problem that the parser parsed
  Problem *extractParsedProblem();
};

#endif
