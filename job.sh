#!/bin/bash
#SBATCH --job-name=Galaxy-Nbody
#SBATCH --output=Galaxy-Nbody.out
#SBATCH --error=Galaxy-Nbody.err
#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=96
#SBATCH --mem=10G

echo "Running on $SLURM_JOB_NODELIST"
echo "Running on $SLURM_JOB_CPUS_PER_NODE cores"
echo "Job ID: $SLURM_JOB_ID"

echo "Compiling..."
make

echo "Running..."
./simulation