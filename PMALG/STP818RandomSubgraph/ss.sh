#!/bin/bash
# author: wuxinyu

STARTTIME=$(date +%s)


./a.out ./instances1/R100K01EFSTINT.stp -timelimit 3600 -puw 1 -pnbase 1000 -partn 0 -parttimes 1 -seed 0 -prep 1 -msit 65536 -mstype 1 -logfilename ./results/R100K01EFSTINT.stp

ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME-$STARTTIME)) seconds to complete this task..."
echo "Done."
