SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
# CPLEX root can be overridden from the environment, e.g.:
# make CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio_Community2212/cplex
CPLEXDIR ?= $(firstword $(wildcard \
	/opt/ibm/ILOG/CPLEX_Studio*/cplex \
	/opt/IBM/ILOG/CPLEX_Studio*/cplex \
	/usr/local/CPLEX_Studio*/cplex))

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CPLEXINCDIR   = $(CPLEXDIR)/include

GUROBIDIR ?= $(firstword $(wildcard \
	/home/jack/opt/gurobi*/linux64 \
	/opt/gurobi*/linux64 \
	/usr/local/gurobi*/linux64))
GUROBILIBDIR  = $(GUROBIDIR)/lib
GUROBIINCDIR  = $(GUROBIDIR)/include

ifeq ($(strip $(CPLEXDIR)),)
$(error CPLEX not found. Install IBM ILOG CPLEX and set CPLEXDIR, e.g. make CPLEXDIR=/opt/ibm/ILOG/CPLEX_Studio_Community2212/cplex)
endif


# CHANGEME: This should be the name of your executable
EXE = Tournaments 

# CHANGEME: Here is the name of all object files corresponding to the source
#           code that you wrote in order to define the problem statement
OBJS = src/Functions.o src/Tournaments.o 

# CHANGEME: Additional libraries
ADDLIBS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(GUROBILIBDIR) -lgurobi130 -Wl,-rpath,$(GUROBILIBDIR) -m64 -lm -lpthread -ldl

# CHANGEME: Additional flags for compilation (e.g., include flags)
DEFINES= -DSYS_UNIX=1 

INCL = -I$(CPLEXINCDIR) -I$(GUROBIINCDIR) -g $(DEFINES)

# C Compiler command
CC = gcc

# C Compiler options
CFLAGS = -m64 -fPIC -fexceptions -O3 -pipe -DNDEBUG -Wall

CYGPATH_W = echo

all: $(EXE)

.SUFFIXES: .c .o .obj

$(EXE): $(OBJS)
	bla=;\
	for file in $(OBJS); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CC) $(CFLAGS) -o $@ $$bla $(LIBS) $(ADDLIBS)

clean:
	rm -f $(EXE) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ $<


.c.obj:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ `$(CYGPATH_W) '$<'`
