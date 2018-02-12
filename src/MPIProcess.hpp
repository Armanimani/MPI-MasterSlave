#pragma once

#include "MPITag.hpp"

#include <mpi.h>
#include <cstddef>

class MPIProcess
{
public:
	MPIProcess(const MPI_Comm& communicator = MPI_COMM_WORLD) : communicator_(communicator)
	{
		MPI_Comm_size(communicator_, &size_);
		MPI_Comm_rank(communicator_, &rank_);
	}

protected:
	const int MASTER_ID_ = 0;
	const MPI_Comm communicator_;
	int size_;
	int rank_;
};