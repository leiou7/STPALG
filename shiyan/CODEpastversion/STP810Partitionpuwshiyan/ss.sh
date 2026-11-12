#!/bin/bash
# author: wuxinyu

STARTTIME=$(date +%s)

Input_File_Name=("./instances/R100K01EFST.stp" "./instances/R100K02EFST.stp" "./instances/R100K03EFST.stp" "./instances/R100K04EFST.stp" "./instances/R100K05EFST.stp" "./instances/R100K06EFST.stp" "./instances/R100K07EFST.stp" "./instances/R100K08EFST.stp" "./instances/R100K09EFST.stp" "./instances/R100K10EFST.stp" "./instances/R100K11EFST.stp" "./instances/R100K12EFST.stp" "./instances/R100K13EFST.stp" "./instances/R100K14EFST.stp" "./instances/R100K15EFST.stp" "./instances/R25K01EFST.stp" "./instances/R25K02EFST.stp" "./instances/R25K03EFST.stp" "./instances/R25K04EFST.stp" "./instances/R25K05EFST.stp" "./instances/R25K06EFST.stp" "./instances/R25K07EFST.stp" "./instances/R25K08EFST.stp" "./instances/R25K09EFST.stp" "./instances/R25K10EFST.stp" "./instances/R25K11EFST.stp" "./instances/R25K12EFST.stp" "./instances/R25K13EFST.stp" "./instances/R25K14EFST.stp" "./instances/R25K15EFST.stp" "./instances/R50K01EFST.stp" "./instances/R50K02EFST.stp" "./instances/R50K03EFST.stp" "./instances/R50K04EFST.stp" "./instances/R50K05EFST.stp" "./instances/R50K06EFST.stp" "./instances/R50K07EFST.stp" "./instances/R50K08EFST.stp" "./instances/R50K09EFST.stp" "./instances/R50K10EFST.stp" "./instances/R50K11EFST.stp" "./instances/R50K12EFST.stp" "./instances/R50K13EFST.stp" "./instances/R50K14EFST.stp" "./instances/R50K15EFST.stp")

Output_File_Name=("./results/R100K01EFST.stp" "./results/R100K02EFST.stp" "./results/R100K03EFST.stp" "./results/R100K04EFST.stp" "./results/R100K05EFST.stp" "./results/R100K06EFST.stp" "./results/R100K07EFST.stp" "./results/R100K08EFST.stp" "./results/R100K09EFST.stp" "./results/R100K10EFST.stp" "./results/R100K11EFST.stp" "./results/R100K12EFST.stp" "./results/R100K13EFST.stp" "./results/R100K14EFST.stp" "./results/R100K15EFST.stp" "./results/R25K01EFST.stp" "./results/R25K02EFST.stp" "./results/R25K03EFST.stp" "./results/R25K04EFST.stp" "./results/R25K05EFST.stp" "./results/R25K06EFST.stp" "./results/R25K07EFST.stp" "./results/R25K08EFST.stp" "./results/R25K09EFST.stp" "./results/R25K10EFST.stp" "./results/R25K11EFST.stp" "./results/R25K12EFST.stp" "./results/R25K13EFST.stp" "./results/R25K14EFST.stp" "./results/R25K15EFST.stp" "./results/R50K01EFST.stp" "./results/R50K02EFST.stp" "./results/R50K03EFST.stp" "./results/R50K04EFST.stp" "./results/R50K05EFST.stp" "./results/R50K06EFST.stp" "./results/R50K07EFST.stp" "./results/R50K08EFST.stp" "./results/R50K09EFST.stp" "./results/R50K10EFST.stp" "./results/R50K11EFST.stp" "./results/R50K12EFST.stp" "./results/R50K13EFST.stp" "./results/R50K14EFST.stp" "./results/R50K15EFST.stp")

#R100K 0-15
#R50K 15-30
#R25K 30-45

puw=1;

#./a.out ./instances/R100K02EFST.stp -timelimit 3600 -puw 0 -pnbase 1000 -logfilename ./results/R100K02EFST.stp -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1


for((i=0;i<3;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=3;i<6;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=6;i<9;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=9;i<12;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=12;i<15;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=15;i<18;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=18;i<21;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=21;i<24;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=24;i<27;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=27;i<30;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=30;i<33;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=33;i<36;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=36;i<39;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=39;i<42;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait

for((i=42;i<45;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait


ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
