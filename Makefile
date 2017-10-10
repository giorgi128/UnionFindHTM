CPP = g++
FLAGS = -mrtm -O3 -std=c++11
FLAGS += -DUSE_GSTATS -DMAX_TID_POW2=128
FLAGS += -I./
FLAGS += -I./Finds
FLAGS += -I./common
FLAGS += -I./include
LDFLAGS = -L./lib -pthread

FLAGS += -DUSE_PAPI
LDFLAGS += -lpapi

all: seq con

con: connocomp conhalving consplitting
connocomp:
	$(CPP) RankConcurrentRun.cpp -o $@.out -DFIND_H_FILE=$@.h $(FLAGS) $(LDFLAGS)
conhalving:
	$(CPP) RankConcurrentRun.cpp -o $@.out -DFIND_H_FILE=$@.h $(FLAGS) $(LDFLAGS)
consplitting:
	$(CPP) RankConcurrentRun.cpp -o $@.out -DFIND_H_FILE=$@.h $(FLAGS) $(LDFLAGS)

seq: seqnocomp seqhalving seqsplitting
seqnocomp:
	$(CPP) RankSequentialRun.cpp -o $@.out -DFIND_H_FILE=$@.h $(FLAGS) $(LDFLAGS)
seqhalving:
	$(CPP) RankSequentialRun.cpp -o $@.out -DFIND_H_FILE=$@.h $(FLAGS) $(LDFLAGS)
seqsplitting:
	$(CPP) RankSequentialRun.cpp -o $@.out -DFIND_H_FILE=$@.h $(FLAGS) $(LDFLAGS)

clean:
	rm -f *.out
