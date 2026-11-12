import os
import sys

bens = []
L = list(sys.argv)


for root, dire, filenames in os.walk('instances'):
    for filename in filenames:
        bens.append(filename)

bens.sort()


i = int(L[1])
ben = bens[i]

arg='srun -p intel-E5-2670' +' ./a.out '+' ./instances/'+ben +' -puw 0'+' -partn 64'+' -seed 0'+' -timelimit 3600'+' -msit 65536'+' -mstpe 1'+' -logfilename'+' ./results/'+ben 

print(ben)
print(arg)
os.system(arg)
