Made by:
	- Sara Machado Nº 86923
	- Ricardo Grade Nº 90774


Directory Structure: 5 directories, 59 files

	ex2-Grupo50
	├── CircuitRouter-ParSolver
	│   ├── CircuitRouter-ParSolver.c
	│   ├── coordinate.c
	│   ├── coordinate.h
	│   ├── grid.c
	│   ├── grid.h
	│   ├── Makefile
	│   ├── maze.c
	│   ├── maze.h
	│   ├── router.c
	│   └── router.h
	├── CircuitRouter-SeqSolver
	│   ├── CircuitRouter-SeqSolver.c
	│   ├── coordinate.c
	│   ├── coordinate.h
	│   ├── grid.c
	│   ├── grid.h
	│   ├── Makefile
	│   ├── maze.c
	│   ├── maze.h
	│   ├── router.c
	│   └── router.h
	├── doTest.sh
	├── inputs
	│   ├── generate.py
	│   ├── random-x128-y128-z3-n128.txt
	│   ├── random-x128-y128-z3-n64.txt
	│   ├── random-x128-y128-z5-n128.txt
	│   ├── random-x256-y256-z3-n256.txt
	│   ├── random-x256-y256-z5-n256.txt
	│   ├── random-x32-y32-z3-n64.txt
	│   ├── random-x32-y32-z3-n96.txt
	│   ├── random-x48-y48-z3-n48.txt
	│   ├── random-x48-y48-z3-n64.txt
	│   ├── random-x64-y64-z3-n48.txt
	│   └── random-x64-y64-z3-n64.txt
	├── lib
	│   ├── commandlinereader.c
	│   ├── commandlinereader.h
	│   ├── list.c
	│   ├── list.h
	│   ├── pair.c
	│   ├── pair.h
	│   ├── queue.c
	│   ├── queue.h
	│   ├── timer.h
	│   ├── types.h
	│   ├── utility.h
	│   ├── vector.c
	│   └── vector.h
	├── Makefile
	├── README.txt
	└── results
	    ├── random-x128-y128-z3-n128.txt.speedups.csv
	    ├── random-x128-y128-z3-n64.txt.speedups.csv
	    ├── random-x128-y128-z5-n128.txt.speedups.csv
	    ├── random-x256-y256-z3-n256.txt.speedups.csv
	    ├── random-x256-y256-z5-n256.txt.speedups.csv
	    ├── random-x32-y32-z3-n64.txt.speedups.csv
	    ├── random-x32-y32-z3-n96.txt.speedups.csv
	    ├── random-x48-y48-z3-n48.txt.speedups.csv
	    ├── random-x48-y48-z3-n64.txt.speedups.csv
	    ├── random-x64-y64-z3-n48.txt.speedups.csv
	    └── random-x64-y64-z3-n64.txt.speedups.csv


To compile:
	all: Run make in /ex2-Grupo50
	CircuitRouter-ParSolver: Run make in /ex2-Grupo50/CircuitRouter-ParSolver
	CircuitRouter-SeqSolver: Run make in /ex2-Grupo50/CircuitRouter-SeqSolver


To Execute:
	doTest.sh: Run ./doTest.sh NumberOfThreads InputPathName in /ex2-Grupo50
	CircuitRouter-ParSolver: Run ./CircuitRouter-ParSolver -t NumberOfThreads InputPathName in /ex2-Grupo50/CircuitRouter-ParSolver
	CircuitRouter-SeqSolver: Run ./CircuitRouter-SeqSolver InputPathName in /ex2-Grupo50/CircuitRouter-SeqSolver


Number of cores: 4

Clock rate: 3.40GHz

Model: Intel(R) Core(TM) i5-3570
