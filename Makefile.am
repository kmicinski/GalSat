# GalSat -- Makefile

AUTOMAKE_OPTIONS = gnu
AM_CPPFLAGS = -g -I minisat-2.2.0 -I galib247
AM_LDFLAGS = 
AM_CFLAGS = -g -Wall
AM_CXXFLAGS =
bin_PROGRAMS = galsat

galsat_SOURCES = galsat.cpp ClauseAllocator.cc Clause.cc DimacsParser.cc Literal.cc Problem.cc VariableAssignment.cc EnumerativeSolver.cc SatSolver.cc LiteralWatcherDatabase.cc Problem.h GSatSolver.cc SimpleGASolver.cc GAVariableAssignmentGenome.cc PartialGASolver.cc GSatPartialSolver.cc PenaltyTracker.cc AdaptivePenaltyGA.cc AdaptivePenaltyGenome.cc AdaptiveGASolver.cc

galsat_LDADD = -L galib247/ga/libga.a

ETAGS=etags

tags:
	-@rm TAGS
	-@find *.cpp *.h *.c *.cc | xargs $(ETAGS)

.PHONY: tags
