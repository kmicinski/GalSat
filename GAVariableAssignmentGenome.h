/*
 * File: GAVariableAssignmentGenome.h
 * Purpose:
 * Implementation of a variable assignment genome
 */

#ifndef _GALSAT_VAR_ASSIGN_GENOME_H_
#define _GALSAT_VAR_ASSIGN_GENOME_H_

#include "Problem.h"
#include "GSatSolver.h"
#include "GSatPartialSolver.h"

#include <ga/ga.h>

class GAVariableAssignmentGenome : public GSatSolver, public GA1DBinaryStringGenome
{
private:
  
protected:
  virtual void flipAndCalculateConflicts(unsigned long variableNumber);
  
public:
  GADefineIdentity("GAVariableAssignmentGenome", 201);
  
  GAVariableAssignmentGenome(Problem *problem);
  GAVariableAssignmentGenome(const GAGenome& g) :GA1DBinaryStringGenome::GA1DBinaryStringGenome((GA1DBinaryStringGenome &)g) {  copy(g); }
  
  virtual GAGenome *clone(CloneMethod) const { return new GAVariableAssignmentGenome(*this); }
  
  // The copy function to copy this genome from another genome
  virtual void copy(const GAGenome &c);
  
  // The evaluation function
  virtual float evaluate(GABoolean flag = gaFalse);
    
  static float staticevaluate(GAGenome &g) { return ((GAVariableAssignmentGenome &)g).evaluate(); }
};

class GAPartialVariableAssignmentGenome : public GSatPartialSolver, public GA1DBinaryStringGenome
{
public:
  GADefineIdentity("GAPartialVariableAssignmentGenome", 202);
  
  GAPartialVariableAssignmentGenome(Problem *problem);
 GAPartialVariableAssignmentGenome(const GAGenome& g) :GA1DBinaryStringGenome::GA1DBinaryStringGenome((GA1DBinaryStringGenome &)g) {  copy(g); }
  
  virtual GAGenome *clone(CloneMethod) const { return new GAPartialVariableAssignmentGenome(*this); }
  
  // The copy function to copy this genome from another genome
  virtual void copy(const GAGenome &c);

  virtual float evaluate(GABoolean flag = gaFalse);

  static float staticevaluate(GAGenome &g) { return ((GAPartialVariableAssignmentGenome &)g).evaluate(); }
  
  virtual void assignAndCalculateConflicts(unsigned long variableNumber, Literal::Polarity polarity);
  
  virtual float currentFitness();
};

#endif
