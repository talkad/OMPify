#!/bin/bash
#PBS -N parse_hpcorpus_c
#PBS -l nodes=1:ppn=1

# module load my_module

# Activate the Conda environment
source activate ompify_env2

cd /homes/talkad/OMPify/database_creator/visualization/hpcorpus_stats/statistics
python debug.py

conda deactivate