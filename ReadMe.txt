copy Find version into findversion.h


compile sequential code with  g++ RankSequentialRun.cpp -o sequentialRun.o -mrtm -O3 -std=c++11
then run ./sequentialRun.o with parameters in the following order:
facebook_combined.txt;CA-AstroPh.txt;ca-CondMat.txt;ca-HepPh.txt; (string consisting of input files from where we read Unite operations)

number of SameSet Operations

seed number



compile concurrent code with g++ RankConcurrentlRun.cpp -o concurrentRun.o -mrtm -O3 -pthread -std=c++11

then run  ./concurrentRun.o with parameters in the following order:

number of SameSet Operations

number of threads

seed number

number of allowed failed CASes(we are not using this parameter in anything currently)



or run numactl --localalloc python Simulator.py and it will generate RunTimes.txt
