#!/bin/bash
#PBS -N clean_cpp
#PBS -l nodes=1:ppn=1

# module load my_module

# Activate the Conda environment
source activate ompify_env2

cd /homes/talkad/OMPify/database_creator/parsers/HPCorpus_parser
python hpcorpus_clean.py

conda deactivate