import subprocess
import timeit
import shlex
#import matplotlib.pyplot as plt


NREPEATS = 10  #number of times to call Dijkstra in order to measure average time
seed = 0
inputstr1 =  "facebook_combined.txt;"
inputstr2 = "CA-AstroPh.txt;"
inputstr3="ca-CondMat.txt;"
inputstr4="ca-HepPh.txt;"
mSameSet = 100000000 #number of SameSet operations
maxCAS = 10 #maximum number of unsuccessful CAS operations in find

aSeq = []
FirstRun = 1
bestSeq = -1

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
        ans = p.communicate(inputstr1+inputstr2+inputstr3+inputstr4+' '+str(mSameSet)+' '+str(seed+i))[0]
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
	
    nthreads = 1

    cmd = "cp Finds/"+typeFind+".h findversion.h"
    args = shlex.split(cmd)

    subprocess.call(args)
    while nthreads <= 32:
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
            ans = p.communicate(inputstr1+inputstr2+inputstr3+inputstr4+' '+str(mSameSet)+' '+ \
								str(nthreads)+' '+str(seed+i)+' '+str(maxCAS))[0]

            aCon = ans.split();
            if (len(aCon) == 0): print str(nthreads)+" "+type+" "+typeFind
            #if (aCon[0] != aSeq[0]): print "Wrong Answer"+type+typeFind
			
            tUnite += float(aCon[1])
            tSameSet += float(aCon[3])
				
        timeU.append(tUnite / NREPEATS)
        timeS.append(tSameSet / NREPEATS)
        nthreads *= 2

    cmd = "rm -f findversion.h"
    args = shlex.split(cmd)
    
    subprocess.call(args)

    return timeU, timeS
		



f = open('RunTimes.txt', 'w')

typeFind = ["seqnocomp", "seqhalving", "seqsplitting"]
type = ["Rank"]

f.write('version #threads runTimeTotal runTimeUnite runTimeSameSet speedup\n')

SeqRunTimes = []

for i in range(0, 1):
    for j in range(0, 3):
        SeqRunTimes.append(RunUnionFindSequential(type[i], typeFind[j]))


print "FINISHED SEQUENTIAL"

typeFind = ["connocomp", "conhalving", "consplitting"]

for i in range(0, 1):
    for j in range(0, 3):
        timeU, timeS = RunUnionFindConcurrent(type[i], typeFind[j]);
        l = 1
        k = 0
        while l <= 32:
            f.write(type[i] + ' ' + typeFind[j]+' '+str(l) + ' ' + str(timeU[k]+timeS[k]) + ' ' + str(timeU[k]) +' ' + str(timeS[k]) + \
							  ' ' + str(SeqRunTimes[3*i+j]  / (timeU[k]+timeS[k])) + '\n')
            l *= 2
            k += 1
		

