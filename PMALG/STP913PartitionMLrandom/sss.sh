#!/bin/bash
# author: wuxinyu

STARTTIME=$(date +%s)

Input_File_Name=("./instances/R100K01EFST.stp" "./instances/R100K02EFST.stp" "./instances/R100K03EFST.stp" "./instances/R100K04EFST.stp" "./instances/R100K05EFST.stp" "./instances/R100K06EFST.stp" "./instances/R100K07EFST.stp" "./instances/R100K08EFST.stp" "./instances/R100K09EFST.stp" "./instances/R100K10EFST.stp" "./instances/R100K11EFST.stp" "./instances/R100K12EFST.stp" "./instances/R100K13EFST.stp" "./instances/R100K14EFST.stp" "./instances/R100K15EFST.stp" "./instances/R25K01EFST.stp" "./instances/R25K02EFST.stp" "./instances/R25K03EFST.stp" "./instances/R25K04EFST.stp" "./instances/R25K05EFST.stp" "./instances/R25K06EFST.stp" "./instances/R25K07EFST.stp" "./instances/R25K08EFST.stp" "./instances/R25K09EFST.stp" "./instances/R25K10EFST.stp" "./instances/R25K11EFST.stp" "./instances/R25K12EFST.stp" "./instances/R25K13EFST.stp" "./instances/R25K14EFST.stp" "./instances/R25K15EFST.stp" "./instances/R50K01EFST.stp" "./instances/R50K02EFST.stp" "./instances/R50K03EFST.stp" "./instances/R50K04EFST.stp" "./instances/R50K05EFST.stp" "./instances/R50K06EFST.stp" "./instances/R50K07EFST.stp" "./instances/R50K08EFST.stp" "./instances/R50K09EFST.stp" "./instances/R50K10EFST.stp" "./instances/R50K11EFST.stp" "./instances/R50K12EFST.stp" "./instances/R50K13EFST.stp" "./instances/R50K14EFST.stp" "./instances/R50K15EFST.stp")

Output_File_Name=("./results/R100K01EFST.stp" "./results/R100K02EFST.stp" "./results/R100K03EFST.stp" "./results/R100K04EFST.stp" "./results/R100K05EFST.stp" "./results/R100K06EFST.stp" "./results/R100K07EFST.stp" "./results/R100K08EFST.stp" "./results/R100K09EFST.stp" "./results/R100K10EFST.stp" "./results/R100K11EFST.stp" "./results/R100K12EFST.stp" "./results/R100K13EFST.stp" "./results/R100K14EFST.stp" "./results/R100K15EFST.stp" "./results/R25K01EFST.stp" "./results/R25K02EFST.stp" "./results/R25K03EFST.stp" "./results/R25K04EFST.stp" "./results/R25K05EFST.stp" "./results/R25K06EFST.stp" "./results/R25K07EFST.stp" "./results/R25K08EFST.stp" "./results/R25K09EFST.stp" "./results/R25K10EFST.stp" "./results/R25K11EFST.stp" "./results/R25K12EFST.stp" "./results/R25K13EFST.stp" "./results/R25K14EFST.stp" "./results/R25K15EFST.stp" "./results/R50K01EFST.stp" "./results/R50K02EFST.stp" "./results/R50K03EFST.stp" "./results/R50K04EFST.stp" "./results/R50K05EFST.stp" "./results/R50K06EFST.stp" "./results/R50K07EFST.stp" "./results/R50K08EFST.stp" "./results/R50K09EFST.stp" "./results/R50K10EFST.stp" "./results/R50K11EFST.stp" "./results/R50K12EFST.stp" "./results/R50K13EFST.stp" "./results/R50K14EFST.stp" "./results/R50K15EFST.stp")

#R100K 0-15
#R50K 15-30
#R25K 30-45
Input_es=("./instances/es10000fst01.stp" "./instances/es1000fst01.stp" "./instances/es1000fst02.stp" "./instances/es1000fst03.stp" "./instances/es1000fst04.stp" "./instances/es1000fst05.stp" "./instances/es1000fst06.stp" "./instances/es1000fst07.stp" "./instances/es1000fst08.stp" "./instances/es1000fst09.stp" "./instances/es1000fst10.stp" "./instances/es1000fst11.stp" "./instances/es1000fst12.stp" "./instances/es1000fst13.stp" "./instances/es1000fst14.stp" "./instances/es1000fst15.stp")
Output_es=("./results/es10000fst01.stp" "./results/es1000fst01.stp" "./results/es1000fst02.stp" "./results/es1000fst03.stp" "./results/es1000fst04.stp" "./results/es1000fst05.stp" "./results/es1000fst06.stp" "./results/es1000fst07.stp" "./results/es1000fst08.stp" "./results/es1000fst09.stp" "./results/es1000fst10.stp" "./results/es1000fst11.stp" "./results/es1000fst12.stp" "./results/es1000fst13.stp" "./results/es1000fst14.stp" "./results/es1000fst15.stp")

#GEO 0-23
Input_geo=("./instances/G101.stp" "./instances/G102.stp" "./instances/G103.stp" "./instances/G104.stp" "./instances/G105.stp" "./instances/G106.stp" "./instances/G107.stp" "./instances/G201.stp" "./instances/G202.stp" "./instances/G203.stp" "./instances/G204.stp" "./instances/G205.stp" "./instances/G206.stp" "./instances/G207.stp" "./instances/G301.stp" "./instances/G302.stp" "./instances/G303.stp" "./instances/G304.stp" "./instances/G305.stp" "./instances/G306.stp" "./instances/G307.stp" "./instances/G308.stp" "./instances/G309.stp")
Output_geo=("./results/G101.stp" "./results/G102.stp" "./results/G103.stp" "./results/G104.stp" "./results/G105.stp" "./results/G106.stp" "./results/G107.stp" "./results/G201.stp" "./results/G202.stp" "./results/G203.stp" "./results/G204.stp" "./results/G205.stp" "./results/G206.stp" "./results/G207.stp" "./results/G301.stp" "./results/G302.stp" "./results/G303.stp" "./results/G304.stp" "./results/G305.stp" "./results/G306.stp" "./results/G307.stp" "./results/G308.stp" "./results/G309.stp")

#I 0-10
Input_I=("./instances/I016.stp" "./instances/I024.stp" "./instances/I026.stp" "./instances/I027.stp" "./instances/I032.stp" "./instances/I046.stp" "./instances/I062.stp" "./instances/I064.stp" "./instances/I075.stp" "./instances/I083.stp")
Output_I=("./results/I016.stp" "./results/I024.stp" "./results/I026.stp" "./results/I027.stp" "./results/I032.stp" "./results/I046.stp" "./results/I062.stp" "./results/I064.stp" "./results/I075.stp" "./results/I083.stp")

#hiv 0-2
Input_hiv=("./instances/hiv-1.stp" "./instances/hiv-2.stp")
Input_hiv=("./results/hiv-1.stp" "./results/hiv-2.stp")

#tspfst 0-10
Input_tspfst=("./instances/d2103fst.stp" "./instances/fl3795fst.stp" "./instances/fnl4461fst.stp" "./instances/pcb3038fst.stp" "./instances/pla7397fst.stp" "./instances/pr2392fst.stp" "./instances/rl11849fst.stp" "./instances/rl5915fst.stp" "./instances/rl5934fst.stp" "./instances/u2319fst.stp")
Output_tspfst=("./results/d2103fst.stp" "./results/fl3795fst.stp" "./results/fnl4461fst.stp" "./results/pcb3038fst.stp" "./results/pla7397fst.stp" "./results/pr2392fst.stp" "./results/rl11849fst.stp" "./results/rl5915fst.stp" "./results/rl5934fst.stp" "./results/u2319fst.stp")



#./a.out ./instances/R100K02EFST.stp -timelimit 3600 -puw 0 -pnbase 1000 -logfilename ./results/R100K02EFST.stp -partn 0 -parttimes 1 -seed 1 -prep 1 -msit 65536 -mstype 1

puw=0;
for((i=0;i<3;i++));do
{
    ./a.out ${Input_File_Name[i]} -timelimit 3600 -puw $puw -pnbase 1000 -logfilename ${Output_File_Name[i]} -partn 0 -seed 0 -prep 1 -msit 65536 -mstype 1
}&
done
wait








ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
