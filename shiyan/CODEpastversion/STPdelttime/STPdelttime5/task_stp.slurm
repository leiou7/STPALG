#!/bin/bash
# task.slurm
#SBATCH --job-name=run
#SBATCH --mail-type=end
#SBATCH --mail-user=yi.zhou@univ-angers.fr
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=24:0:0
#SBATCH --mem-per-cpu=8G
#SBATCH -p intel-E5-2695

# Dynamic Linking Library
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/yi.zhou/Gurobi_Env/gurobi811/linux64/lib

# your bash script
# you could give the control to python or whatever you like

python3 run_stp.py ${SLURM_ARRAY_TASK_ID} $1