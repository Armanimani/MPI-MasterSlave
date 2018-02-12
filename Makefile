all:
	mpicxx src/MPIMasterProcess.cpp src/MPISlaveProcess.cpp test/IntegralMaster.cpp test/IntegralSlave.cpp test/main.cpp -std=c++1z

run:
	mpiexec -np 8 ./a.out