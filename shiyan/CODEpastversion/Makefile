#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio201/cplex
CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio201/concert
# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEX_LIB=$(CPLEXDIR)/lib/x86-64_linux/static_pic
CONCERT_LIB=$(CONCERTDIR)/lib/x86-64_linux/static_pic

# For dynamic linking
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIB      = cplex$(dynamic:yes=2010)
run           = $(dynamic:yes=LD_LIBRARY_PATH=$(CPLEXBINDIR))

CCLNDIRS  = -L$(CPLEX_LIB) -L$(CONCERT_LIB) $(dynamic:yes=-L$(CPLEXBINDIR))
CLNDIRS   = -L$(CPLEX_LIB) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -lm -lpthread -ldl
CLNFLAGS  = -l$(CPLEXLIB) -lm -lpthread -ldl

# ---------------------------------------------------------------------
#include flag
# ---------------------------------------------------------------------
all:
	make all_cpp
execute: all
	make execute_cpp

EXDATA        = ./src
EXSRCCPP      = ./src

CPLEX_INCLUDE=$(CPLEXDIR)/include
CONCERT_INCLUDE=$(CONCERTDIR)/include
CCFLAGS = $(CCOPT) -I$(CPLEX_INCLUDE) -I$(CONCERT_INCLUDE) 
JCFLAGS = $(JOPT)
#------------------------------------------------------------
#  make all      : to compile the examples. 
#  make execute  : to compile and execute the examples.
#------------------------------------------------------------
CPP_EX = ktree ktreef ktreew kbd kbdf kbdw kbc kbcf kbcw matroid
all_cpp: $(CPP_EX)
execute_cpp: $(CPP_EX)
	$(run) ./ktree
	$(run) ./ktreef
	$(run) ./kbd
	$(run) ./kbdf
	$(run) ./kbc
	$(run) ./kbcf
	$(run) ./matroid
	$(run) ./ktreefreg
	$(run) ./kbdfreg
	$(run) ./kbcfreg
	$(run) ./ktreew
	$(run) ./kbdw
	$(run) ./kbcw
	$(run) ./reduction





# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~ *.class
	/bin/rm -rf $(C_EX) $(CX_EX) $(CPP_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

# ------------------------------------------------------------
ktree: ktree.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o ktree ktree.o $(CCLNFLAGS)
ktree.o: $(EXSRCCPP)/ktree.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/ktree.cpp -o ktree.o

ktreef: ktreef.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o ktreef ktreef.o $(CCLNFLAGS)
ktreef.o: $(EXSRCCPP)/ktreef.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/ktreef.cpp -o ktreef.o

kbd: kbd.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbd kbd.o $(CCLNFLAGS)
kbd.o: $(EXSRCCPP)/kbd.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbd.cpp -o kbd.o

kbdf: kbdf.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbdf kbdf.o $(CCLNFLAGS)
kbdf.o: $(EXSRCCPP)/kbdf.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbdf.cpp -o kbdf.o

kbc: kbc.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbc kbc.o $(CCLNFLAGS)
kbc.o: $(EXSRCCPP)/kbc.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbc.cpp -o kbc.o

kbcf: kbcf.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbcf kbcf.o $(CCLNFLAGS)
kbcf.o: $(EXSRCCPP)/kbcf.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbcf.cpp -o kbcf.o

matroid: matroid.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o matroid matroid.o $(CCLNFLAGS)
matroid.o: $(EXSRCCPP)/matroid.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/matroid.cpp -o matroid.o

ktreefreg: ktreefreg.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o ktreefreg ktreefreg.o $(CCLNFLAGS)
ktreefreg.o: $(EXSRCCPP)/ktreefreg.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/ktreefreg.cpp -o ktreefreg.o

kbdfreg: kbdfreg.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbdfreg kbdfreg.o $(CCLNFLAGS)
kbdfreg.o: $(EXSRCCPP)/kbdfreg.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbdfreg.cpp -o kbdfreg.o

kbcfreg: kbcfreg.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbcfreg kbcfreg.o $(CCLNFLAGS)
kbcfreg.o: $(EXSRCCPP)/kbcfreg.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbcfreg.cpp -o kbcfreg.o

ktreew: ktreew.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o ktreew ktreew.o $(CCLNFLAGS)
ktreew.o: $(EXSRCCPP)/ktreew.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/ktreew.cpp -o ktreew.o

kbdw: kbdw.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbdw kbdw.o $(CCLNFLAGS)
kbdw.o: $(EXSRCCPP)/kbdw.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbdw.cpp -o kbdw.o

kbcw: kbcw.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o kbcw kbcw.o $(CCLNFLAGS)
kbcw.o: $(EXSRCCPP)/kbcw.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/kbcw.cpp -o kbcw.o

reduction: reduction.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o reduction reduction.o $(CCLNFLAGS)
reduction.o: $(EXSRCCPP)/reduction.cpp
	$(CCC) -c $(CCFLAGS) $(EXSRCCPP)/reduction.cpp -o reduction.o


# Local Variables:
# mode: makefile
# End:





