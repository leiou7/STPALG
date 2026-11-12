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

arg='srun -p intel-E5-2670' +' ./a.out '+' ./instances/'+ben +' -timelimit 3600'+' -puw 0'+' -connect 1'+' -partn 64'+' -fbase 2'+' -delttime 20'+' -logfilename'+' ./results/'+ben+' -seed 0'+' -prep 1' 

print(ben)
print(arg)
os.system(arg)
