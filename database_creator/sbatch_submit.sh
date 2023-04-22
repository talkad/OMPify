#!/bin/bash

#SBATCH -p mixedp
#SBATCH -o slurm-%j.out
#SBATCH --error=slurm-%j.err

cd /home/talkad/Downloads/thesis/data_gathering_script
python main.py --parse "(c)" | tee output.log