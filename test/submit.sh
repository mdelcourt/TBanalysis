#!/bin/bash
#
#SBATCH --job-name=alignment
#SBATCH --output=results_%t.txt
#
#SBATCH --ntasks=1
#SBATCH --time=59:59
#SBATCH --mem-per-cpu=1500
#
#SBATCH --array=0-8

srun python pipeline.py $SLURM_ARRAY_TASK_ID
