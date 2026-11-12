#!/bin/bash
# author: wuxinyu

STARTTIME=$(date +%s)

#R100K 0-15
#R50K 15-30
#R25K 30-45
Input_File_Name=("./instances/R100K01EFST.stp" "./instances/R100K02EFST.stp" "./instances/R100K03EFST.stp" "./instances/R100K04EFST.stp" "./instances/R100K05EFST.stp" "./instances/R100K06EFST.stp" "./instances/R100K07EFST.stp" "./instances/R100K08EFST.stp" "./instances/R100K09EFST.stp" "./instances/R100K10EFST.stp" "./instances/R100K11EFST.stp" "./instances/R100K12EFST.stp" "./instances/R100K13EFST.stp" "./instances/R100K14EFST.stp" "./instances/R100K15EFST.stp" "./instances/R25K01EFST.stp" "./instances/R25K02EFST.stp" "./instances/R25K03EFST.stp" "./instances/R25K04EFST.stp" "./instances/R25K05EFST.stp" "./instances/R25K06EFST.stp" "./instances/R25K07EFST.stp" "./instances/R25K08EFST.stp" "./instances/R25K09EFST.stp" "./instances/R25K10EFST.stp" "./instances/R25K11EFST.stp" "./instances/R25K12EFST.stp" "./instances/R25K13EFST.stp" "./instances/R25K14EFST.stp" "./instances/R25K15EFST.stp" "./instances/R50K01EFST.stp" "./instances/R50K02EFST.stp" "./instances/R50K03EFST.stp" "./instances/R50K04EFST.stp" "./instances/R50K05EFST.stp" "./instances/R50K06EFST.stp" "./instances/R50K07EFST.stp" "./instances/R50K08EFST.stp" "./instances/R50K09EFST.stp" "./instances/R50K10EFST.stp" "./instances/R50K11EFST.stp" "./instances/R50K12EFST.stp" "./instances/R50K13EFST.stp" "./instances/R50K14EFST.stp" "./instances/R50K15EFST.stp")
Output_File_Name=("./results/R100K01EFST.stp" "./results/R100K02EFST.stp" "./results/R100K03EFST.stp" "./results/R100K04EFST.stp" "./results/R100K05EFST.stp" "./results/R100K06EFST.stp" "./results/R100K07EFST.stp" "./results/R100K08EFST.stp" "./results/R100K09EFST.stp" "./results/R100K10EFST.stp" "./results/R100K11EFST.stp" "./results/R100K12EFST.stp" "./results/R100K13EFST.stp" "./results/R100K14EFST.stp" "./results/R100K15EFST.stp" "./results/R25K01EFST.stp" "./results/R25K02EFST.stp" "./results/R25K03EFST.stp" "./results/R25K04EFST.stp" "./results/R25K05EFST.stp" "./results/R25K06EFST.stp" "./results/R25K07EFST.stp" "./results/R25K08EFST.stp" "./results/R25K09EFST.stp" "./results/R25K10EFST.stp" "./results/R25K11EFST.stp" "./results/R25K12EFST.stp" "./results/R25K13EFST.stp" "./results/R25K14EFST.stp" "./results/R25K15EFST.stp" "./results/R50K01EFST.stp" "./results/R50K02EFST.stp" "./results/R50K03EFST.stp" "./results/R50K04EFST.stp" "./results/R50K05EFST.stp" "./results/R50K06EFST.stp" "./results/R50K07EFST.stp" "./results/R50K08EFST.stp" "./results/R50K09EFST.stp" "./results/R50K10EFST.stp" "./results/R50K11EFST.stp" "./results/R50K12EFST.stp" "./results/R50K13EFST.stp" "./results/R50K14EFST.stp" "./results/R50K15EFST.stp")

#GEO 0-23
Input_geo=("./instances/G101.stp" "./instances/G102.stp" "./instances/G103.stp" "./instances/G104.stp" "./instances/G105.stp" "./instances/G106.stp" "./instances/G107.stp" "./instances/G201.stp" "./instances/G202.stp" "./instances/G203.stp" "./instances/G204.stp" "./instances/G205.stp" "./instances/G206.stp" "./instances/G207.stp" "./instances/G301.stp" "./instances/G302.stp" "./instances/G303.stp" "./instances/G304.stp" "./instances/G305.stp" "./instances/G306.stp" "./instances/G307.stp" "./instances/G308.stp" "./instances/G309.stp")
Output_geo=("./results/G101.stp" "./results/G102.stp" "./results/G103.stp" "./results/G104.stp" "./results/G105.stp" "./results/G106.stp" "./results/G107.stp" "./results/G201.stp" "./results/G202.stp" "./results/G203.stp" "./results/G204.stp" "./results/G205.stp" "./results/G206.stp" "./results/G207.stp" "./results/G301.stp" "./results/G302.stp" "./results/G303.stp" "./results/G304.stp" "./results/G305.stp" "./results/G306.stp" "./results/G307.stp" "./results/G308.stp" "./results/G309.stp")


#I 0-10
Input_I=("./instances/I002.stp" "./instances/I015.stp" "./instances/I016.stp" "./instances/I024.stp" "./instances/I027.stp" "./instances/I044.stp" "./instances/I061.stp" "./instances/I062.stp" "./instances/I064.stp" "./instances/I083.stp")
Output_I=("I002.stp" "I015.stp" "I016.stp" "./results/I024.stp" "./results/I027.stp" "./results/I044.stp" "./results/I061.stp" "./results/I062.stp" "./results/I064.stp" "./results/I083.stp")


#es 0-11
Input_es=("./instances/es10000fst01.stp" "./instances/es1000fst01.stp" "./instances/es1000fst02.stp" "./instances/es1000fst03.stp" "./instances/es1000fst04.stp" "./instances/es1000fst05.stp" "./instances/es1000fst06.stp" "./instances/es1000fst07.stp" "./instances/es1000fst08.stp" "./instances/es1000fst09.stp" "./instances/es1000fst10.stp" "./instances/es1000fst11.stp" "./instances/es1000fst12.stp" "./instances/es1000fst13.stp" "./instances/es1000fst14.stp" "./instances/es1000fst15.stp")
Output_es=("./results/es10000fst01.stp" "./results/es1000fst01.stp" "./results/es1000fst02.stp" "./results/es1000fst03.stp" "./results/es1000fst04.stp" "./results/es1000fst05.stp" "./results/es1000fst06.stp" "./results/es1000fst07.stp" "./results/es1000fst08.stp" "./results/es1000fst09.stp" "./results/es1000fst10.stp" "./results/es1000fst11.stp" "./results/es1000fst12.stp" "./results/es1000fst13.stp" "./results/es1000fst14.stp" "./results/es1000fst15.stp")


#tspfst 0-10
Input_tspfst=("./instances/fnl4461fst.stp" "./instances/nrw1379fst.stp" "./instances/fl1400fst.stp" "./instances/fl3795fst.stp" "./instances/rat783fst.stp" "./instances/rat575fst.stp" "./instances/pcb3038fst.stp" "./instances/dsj1000fst.stp" "./instances/vm1748fst.stp" "./instances/att532fst.stp")
Output_tspfst=("./results/fnl4461fst.stp" "./results/nrw1379fst.stp" "./results/fl1400fst.stp" "./results/fl3795fst.stp" "./results/rat783fst.stp" "./results/rat575fst.stp" "./results/pcb3038fst.stp" "./results/dsj1000fst.stp" "./results/vm1748fst.stp" "./results/att532fst.stp")

#VLSI 0-10
Input_VLSI=("./instances/alue7065.stp" "./instances/alut2625.stp" "./instances/alut2610.stp" "./instances/alue7080.stp" "./instances/taq0377.stp" "./instances/taq0903.stp" "./instances/alue7066.stp" "./instances/taq0014.stp" "./instances/alut2288.stp" "./instances/alue5345.stp")
Output_VLSI=("./results/alue7065.stp" "./results/alut2625.stp" "./results/alut2610.stp" "./results/alue7080.stp" "./results/taq0377.stp" "./results/taq0903.stp" "./results/alue7066.stp" "./results/taq0014.stp" "./results/alut2288.stp" "./results/alue5345.stp")


#small instances
Input_small=("./instances/2r211.stp" "./instances/alue7080.stp" "./instances/alut2625.stp" "./instances/bipa2p.stp" "./instances/bipa2u.stp" "./instances/cc12-2n.stp" "./instances/cc12-2p.stp" "./instances/cc12-2u.stp" "./instances/cc3-12n.stp" "./instances/cc3-12p.stp" "./instances/cc3-12u.stp" "./instances/d18.stp" "./instances/e18.stp" "./instances/es10000fst01.stp" "./instances/fnl4461fst.stp" "./instances/G106a.stp" "./instances/hc12p.stp" "./instances/hc12u.stp" "./instances/I064a.stp" "./instances/i640-211.stp" "./instances/i640-314.stp" "./instances/i640-341.stp" "./instances/lin36.stp" "./instances/lin37.stp" "./instances/rc09.stp" "./instances/rt05.stp" "./instances/s5.stp" "./instances/w23c23.stp" "./instances/world666.stp" "./instances/wrp3-83.stp")
Output_small=("./results/2r211.stp" "./results/alue7080.stp" "./results/alut2625.stp" "./results/bipa2p.stp" "./results/bipa2u.stp" "./results/cc12-2n.stp" "./results/cc12-2p.stp" "./results/cc12-2u.stp" "./results/cc3-12n.stp" "./results/cc3-12p.stp" "./results/cc3-12u.stp" "./results/d18.stp" "./results/e18.stp" "./results/es10000fst01.stp" "./results/fnl4461fst.stp" "./results/G106a.stp" "./results/hc12p.stp" "./results/hc12u.stp" "./results/I064a.stp" "./results/i640-211.stp" "./results/i640-314.stp" "./results/i640-341.stp" "./results/lin36.stp" "./results/lin37.stp" "./results/rc09.stp" "./results/rt05.stp" "./results/s5.stp" "./results/w23c23.stp" "./results/world666.stp" "./results/wrp3-83.stp")

puw=0;
connect=1;
partn=2;
delttime=1;
fbase=2;
timelimit=10;

#puw 0,1
#connect 1,2,3
#partn initial num of subgraphs
#delttime search time for one subgraph
for((c=1;c<3;c++));do
{
for((i=0;i<3;i++));do
{
    ./a.out ${Input_I[i]} -timelimit $timelimit -puw $puw -connect $c -partn $partn -fbase $fbase -delttime $delttime -logfilename ./results/${c}_${Output_I[i]} -seed 0 -prep 1
}&
done
wait
}
done



ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
