CPP = g++
FLAGS = -mrtm -O3 -std=c++11
FLAGS += -DUSE_GSTATS -DMAX_TID_POW2=128
FLAGS += -I./
FLAGS += -I./Finds
FLAGS += -I./common
FLAGS += -I./include
LDFLAGS = -L./lib -pthread

#FLAGS += -DUSE_PAPI
#LDFLAGS += -lpapi

#XX=g++
#CXXFLAGS=
#LDFLAGS=
#CPPFILE=main.cpp

UNIONBY=rank random
MODE=seq con
COMPRESSBY=nocomp splitting halving

#seq.rank.splitting
all: $(foreach mode,$(MODE),$(foreach unionby,$(UNIONBY),$(foreach compressby,$(COMPRESSBY),$(mode).$(unionby).$(compressby))))

define CREATETARGETS
$(1).$(2).$(3):
	$(CPP) $(1).$(2).cpp  -o $(1).$(2).$(3).out -DFIND_H_FILE=$(1)$(3).h $(FLAGS) $(LDFLAGS)
endef
$(foreach mode,$(MODE),$(foreach unionby,$(UNIONBY),$(foreach compressby,$(COMPRESSBY), $(eval $(call CREATETARGETS,$(mode),$(unionby),$(compressby))))))

clean:
	rm *.out
