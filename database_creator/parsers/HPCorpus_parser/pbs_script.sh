#!/bin/bash
#PBS -N parse_hpcorpus_f
#PBS -l nodes=1:ppn=160

# module load my_module

# Activate the Conda environment
source activate ompify_env2

cd /homes/talkad/OMPify/database_creator/parsers/HPCorpus_parser
python parse_llvm.py

conda deactivate