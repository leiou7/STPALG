#!/bin/bash
# author: wuxinyu

STARTTIME=$(date +%s)

path="./instances"
files=$(ls $path)

declare -a array
declare -i i=0
for filename in $files
do
{
    a[$i]=$filename
    let i=i+1
}
done

siz=${#a[*]}
echo ${siz}


puw=1;

connect=1;
fbase=2;
partn=64;

delttime=20;
timelimit=3600;
#puw 0,1
#connect 1,2
#partn initial num of subgraphs
#delttime search time for one subgraph



./a.out ./instances/R100K01EFST.stp -timelimit $timelimit -puw $puw -connect $c -partn $p -fbase $f -delttime $delttime -logfilename ./results/R100K01EFST.stp -seed 0 -prep 1




ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
