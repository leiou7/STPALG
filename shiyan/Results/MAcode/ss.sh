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

for((i=0;i<${siz};i+=115))
do
{
    for((j=$i;j<$i+15;j++))
    do
    {
          #echo $j     ./instances/${a[j]}    ./results/${a[j]} >> "aaa.txt"
          ./a.out ./instances/${a[j]}  ./results/${a[j]}  3600
    }&
    done
    wait
}
done




ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
