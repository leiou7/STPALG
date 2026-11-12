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


puw=0;

connect=1;
fbase=2;
partn=64;

delttime=20;
timelimit=3600;
#puw 0,1
#connect 1,2
#partn initial num of subgraphs
#delttime search time for one subgraph

for((i=0;i<${siz};i++))
do
{
    for((c=1;c<3;c++))
    do
    {

        for((f=2;f<17;f=f*2))
        do
        {
            for((p=16;p<65;p=p*2))
            do
            {
                #echo ./results/c${c}_f${f}_p${p}_${a[i]} >> "aaa.txt"
                ./a.out ./instances/${a[i]} -timelimit $timelimit -puw $puw -connect $c -partn $p -fbase $f -delttime $delttime -logfilename ./results/c${c}_f${f}_p${p}_${a[i]} -seed 0 -prep 1
            }&
            done
            wait
        }&
        done
        wait
    }
    done
}
done




ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
