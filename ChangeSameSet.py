import subprocess
import timeit
import shlex
#import matplotlib.pyplot as plt
#import matplotlib.pyplot as plt
#import matplotlib.patches as mpatches

NREPEATS = 10  #number of times to call Dijkstra in order to measure average time
seed = 0
inputstr1 =  "facebook_combined.txt;"
inputstr2 = "CA-AstroPh.txt;"
inputstr3="ca-CondMat.txt;"
inputstr4="ca-HepPh.txt;"
inputstr5="roadNet-CA.txt;"
mSameSet = 100000 #number of SameSet operations
maxCAS = 100  #maximum number of unsuccessful CAS operations in find
SameSetVals = [0, 100000, 200000, 500000, 1000000, 2000000, 4000000, 8000000, 10000000, 20000000, 30000000, 40000000, 50000000]

aSeq = []
FirstRun = 1

def RunUnionFindSequential(type, typeFind):
    cmd = "cp Finds/"+typeFind+".h findversion.h"
    args = shlex.split(cmd)
    
    subprocess.call(args)
    
    cmd = "g++ "+type+"SequentialRun.cpp -o sequentialRun.o -mrtm -O3 -std=c++11"
    args = shlex.split(cmd)

    subprocess.call(args)
    
    cmd = "./sequentialRun.o"
    args = shlex.split(cmd)

    timeSeq = 0.0

    curSeq = []

    for i in range (0, NREPEATS):
        p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        ans = p.communicate(inputstr1+inputstr2+inputstr3+inputstr4+inputstr5+' '+str(mSameSet)+' '+str(seed+i))[0]
        curSeq = ans.split()
        
        global FirstRun
        global aSeq
        
        if (FirstRun == 0 and curSeq[0] != aSeq[0]): print "Wrong Answer"+type+typeFind
        if (FirstRun == 1): aSeq = curSeq
        
        FirstRun = 0
        
        timeSeq += float(curSeq[1])

    cmd = "rm -f findversion.h"
    args = shlex.split(cmd)

    subprocess.call(args)

    return timeSeq / NREPEATS


def RunUnionFindConcurrent(type, typeFind):
	
    timeU = []
    timeS = []
	
    nthreads = 10

    cmd = "cp Finds/"+typeFind+".h findversion.h"
    args = shlex.split(cmd)

    subprocess.call(args)
    
    while nthreads <= 10:
		#run concurrent UnionFind with nthreads #threads
        cmd = "g++ "+type+"ConcurrentRun.cpp -o concurrentRun.o -mrtm -pthread -O3 -std=c++11"
        args = shlex.split(cmd)

        subprocess.call(args)

        tUnite = 0.0
        tSameSet = 0.0
		
        cmd = "./concurrentRun.o"
        args = shlex.split(cmd)
		
        for i in range(0, NREPEATS):
			
            p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
            ans = p.communicate(inputstr1+inputstr2+inputstr3+inputstr4+inputstr5+' '+str(mSameSet)+' '+ \
								str(nthreads)+' '+str(seed+i)+' '+str(maxCAS))[0]

            aCon = ans.split();
			
            if (aCon[0] != aSeq[0]): print "Wrong Answer"+type+typeFind
			
            tUnite += float(aCon[1])
            tSameSet += float(aCon[2])
				
        timeU.append(tUnite / NREPEATS)
        timeS.append(tSameSet / NREPEATS)
        nthreads *= 2

    cmd = "rm -f findversion.h"
    args = shlex.split(cmd)
    
    subprocess.call(args)

    return timeU, timeS
		



f = open('RunTimes.txt', 'w')

typeFind = ["seqnocomp", "seqhalving", "seqsplitting"]
type = ["Random", "Rank"]


SeqRunTimes = []
#Speedup = []

lenSameSetVals = len(SameSetVals)

bestSeq = []

for q in range(0,lenSameSetVals):
    mSameSet=SameSetVals[q]
    bestSeq.append(-1)
    for i in range(0, 2):
        for j in range(0, 3):
            cur = RunUnionFindSequential(type[i], typeFind[j])
            if (bestSeq[q] == -1 or cur < bestSeq[q]): bestSeq[q] = cur


print "FINISHED SEQUENTIAL"

typeFind = ["connocomp", "conhalving", "consplitting"]


for q in range(0,lenSameSetVals):
    mSameSet=SameSetVals[q]
    for i in range(0, 2):
        for j in range(0, 3):
            timeU, timeS = RunUnionFindConcurrent(type[i], typeFind[j]);
            l = 1
            k = 0
            while l <= 1:
                f.write(str(bestSeq[q] / (timeU[k]+timeS[k]))+'\n')
                l *= 2
                k += 1

#colors = ['r', 'g', 'y', 'b', 'm', 'k']

#f = open('RunTimes.txt', 'w')
#plt.xlabel('number of sameset operations')
#plt.ylabel('speedup')

#color_patches = []

#for i in range(0, 2):
#    for j in range(0,3):
#        A = []
#        B = []
#        for q in range(0,lenSameSetVals):
#            A.append(SameSetVals[q])
#            B.append(Speedup[q*6+3*i+j])
#        plt.plot(A, B, '.'+colors[3*i+j]+'-')
#        color_patch = mpatches.Patch(color=colors[3*i+j], label=type[i]+typeFind[j])
#        color_patches.append(color_patch)

#plt.legend(handles=color_patches, loc=2)
#plt.show()
		

