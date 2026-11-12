import os
import sys

bens = []
L = list(sys.argv)


for root, dire, filenames in os.walk('instances1'):
    for filename in filenames:
        bens.append(filename)

bens.sort()


i = int(L[1])
ben = bens[i]

arg='srun -p intel-E5-2670' +' ./a.out '+' ./instances1/'+ben +' -puw 1'+" -partn 0"' -pnbase 1000'+' -parttimes 1'+' -prep 1'+' -seed 0'+' -timelimit 300'+' -msit 65536'+' -mstpe 1'+' -logfilename'+' ./results/'+ben 
         
print(ben)
print(arg)
os.system(arg)
