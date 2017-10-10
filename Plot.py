import subprocess
import timeit
import shlex
#import matplotlib.pyplot as plt
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

f = open('RunTimes.txt', 'r')

typeFind = ["nocompaction", "halving", "splitting"]
type = ["Random", "Rank"]

SameSetVals = [0, 100000, 200000, 500000, 1000000, 2000000, 4000000, 8000000, 10000000, 20000000, 30000000, 40000000,  50000000]
lenSameSetVals = len(SameSetVals)

Speedup = []

for q in range(0,lenSameSetVals):
    for i in range(0, 2):
        for j in range(0, 3):
            str=f.readline()
            ar = str.split('\n')
            #while (isFloat(str)==False): str=str[:-1]
            Speedup.append(float(ar[0]))
            print ar[0]

colors = ['r', 'g', 'y', 'b', 'm', 'k']

#f = open('RunTimes1.txt', 'w')
plt.xlabel('number of sameset operations')
plt.ylabel('speedup')

color_patches = []

for i in range(0, 2):
    for j in range(0,3):
        A = []
        B = []
        for q in range(0,lenSameSetVals):
            A.append(SameSetVals[q])
            B.append(Speedup[q*6+3*i+j])
        plt.plot(A, B, '.'+colors[3*i+j]+'-')
        color_patch = mpatches.Patch(color=colors[3*i+j], label=type[i]+typeFind[j])
        color_patches.append(color_patch)


labels = [line.get_label() for line in color_patches]

plt.legend(color_patches, labels, loc=4)
plt.show()
		

