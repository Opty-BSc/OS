#!/bin/bash

#Verifying if a valid number of arguments was given
if [ "$#" -ne 2 ] ;then
	>&2 echo "Invalid number of arguments"
	exit 1
fi

#Inputs
nThreads=$1
fInput=$2

#Verifying if a valid number of threads was given
if [ "$nThreads" -le 0 ] ;then
	>&2 echo "Invalid number of threads"
	exit 1
fi

#Verifying if the Inputfile given already exists
if [ ! -e "$fInput" ] ;then
	>&2 echo "File does not exist"
	exit 1
fi

#Files needed
seqSolver=CircuitRouter-SeqSolver/CircuitRouter-SeqSolver
parSolver=CircuitRouter-ParSolver/CircuitRouter-ParSolver

#Verifying if the Files needed are compiled
if [ ! -e "$seqSolver" ] || [ ! -e "$parSolver" ] ;then
	>&2 echo "CircuitRouter is not compiled"
	exit 1
fi

#Creating Outputfile
fOutput=${fInput}.speedups.csv
echo "#threads,exec_time,speedup" > $fOutput

#Running CircuitRouter-SeqSolver (Sequential)
./$seqSolver $fInput
elapsedSeq=$(grep "Elapsed" ${fInput}.res | grep -Eo "[[:digit:].]+")
echo "1S,$elapsedSeq,1" >> $fOutput

#Running CircuitRouter-ParSolver (With a different number of threads)
for i in $(seq 1 $nThreads)
do
	./$parSolver $fInput -t $i
	elapsedPar=$(grep "Elapsed" ${fInput}.res | grep -Eo "[[:digit:].]+")
	#Calculating speedup
	speedup=$(echo "scale=6; $elapsedSeq/$elapsedPar" | bc)
	echo "$i,$elapsedPar,$speedup" >> $fOutput
done
