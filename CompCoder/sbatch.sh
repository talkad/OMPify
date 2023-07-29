#!/bin/bash
#SBATCH --job-name=pretrain_fortran        # Job name (you can choose any name)
#SBATCH --partition=hpci-h100              # Specify the partition or queue name
#SBATCH --nodes=1                          # Number of nodes to request


source activate ompify_env2

cd /homes/talkad/OMPify/CompCoder

python main.py \
        --langs fortran \
        --do-pre-train \
        --pre-train-tasks mass \
        --batch-size 64 \
        --eval-batch-size 64 \
        --no-replaced \
        --no-ast \
        --no-dfg \
        --model-name pre_train_fortran_mass \
        --fp16
        
        # --fp16 \
        # --cuda-visible-devices 0,1,2,3 \

conda deactivate




----------------------------------------------------------- /opt/cray/pe/perftools/22.12.0/modulefiles -----------------------------------------------------------
perftools             perftools-lite        perftools-lite-events perftools-lite-gpu    perftools-lite-hbm    perftools-lite-loops  perftools-preload

-------------------------------------------------------------------- /opt/cray/pe/modulefiles --------------------------------------------------------------------
PrgEnv-amd/8.3.3(default)                    cray-fftw/3.3.10.3(default)                  cray-ucx/2.7.0-1
PrgEnv-aocc/8.3.3(default)                   cray-hdf5/1.12.2.1(default)                  cray-ucx/default
PrgEnv-cray/8.3.3(default)                   cray-hdf5-parallel/1.12.2.1(default)         craype/2.7.19(default)
PrgEnv-cray-amd/8.3.3                        cray-libpals/1.2.5(default)                  craype-dl-plugin-ftr/22.06.1.2(default)
PrgEnv-gnu/8.3.3(default)                    cray-libsci/22.12.1.1(default)               craype-dl-plugin-py3/21.02.1.3
PrgEnv-gnu-amd/8.3.3                         cray-libsci_acc/22.12.1.1(default)           craype-dl-plugin-py3/21.04.1
PrgEnv-intel/8.3.3(default)                  cray-mpich/8.1.23(default)                   craype-dl-plugin-py3/22.06.1.2
PrgEnv-nvhpc/8.3.3(default)                  cray-mpich-abi/8.1.23(default)               craype-dl-plugin-py3/22.08.1
PrgEnv-nvidia/8.3.3(default)                 cray-mpich-abi-pre-intel-5.0/8.1.23(default) craype-dl-plugin-py3/22.09.1
atp/3.14.16(default)                         cray-mpich-ucx/8.1.23(default)               craype-dl-plugin-py3/22.12.1(default)
cce/15.0.0(default)                          cray-mpixlate/1.0.0.6                        craypkg-gen/1.3.28(default)
cpe/22.12(default)                           cray-mrnet/5.0.4(default)                    gcc/10.3.0
cpe-cuda/22.12(default)                      cray-netcdf/4.9.0.1(default)                 gcc/11.2.0
cray-R/4.2.1.1(default)                      cray-netcdf-hdf5parallel/4.9.0.1(default)    gcc/12.2.0(default)
cray-ccdb/4.12.13(default)                   cray-openshmemx/11.5.7(default)              gdb4hpc/4.14.6(default)
cray-cti/2.15.14                             cray-pals/1.2.5(default)                     iobuf/2.0.10(default)
cray-cti/2.16.0                              cray-parallel-netcdf/1.12.3.1(default)       papi/6.0.0.17(default)
cray-cti/2.17.1(default)                     cray-pmi/6.1.8(default)                      perftools-base/22.12.0(default)
cray-dsmml/0.2.2(default)                    cray-python/3.9.13.1(default)                sanitizers4hpc/1.0.4(default)
cray-dyninst/12.1.1(default)                 cray-stat/4.11.13(default)                   valgrind4hpc/2.12.10(default)

--------------------------------------------------------------------- /opt/cray/modulefiles ----------------------------------------------------------------------
cudatoolkit/23.1_12.0

------------------------------------------------------------------------ /opt/modulefiles ------------------------------------------------------------------------
nvhpc/23.1(default)  nvidia/23.1(default)

-------------------------------------------------------- /opt/cray/pe/craype-targets/default/modulefiles ---------------------------------------------------------
craype-accel-amd-gfx90a craype-accel-nvidia70   craype-network-none     craype-x86-genoa        craype-x86-rome         craype-x86-trento
craype-accel-amd-gfx908 craype-accel-nvidia80   craype-network-ofi      craype-x86-milan        craype-x86-spr
craype-accel-host       craype-arm-grace        craype-network-ucx      craype-x86-milan-x      craype-x86-spr-hbm
