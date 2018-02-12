#pragma once

#include "MPIMasterProcess.hpp"
#include "MPISlaveProcess.hpp"

#include <mpi.h>
#include <type_traits>

template <class Master, class Slave, typename = std::enable_if_t<std::is_base_of_v<MPIMasterProcess, Master>, void>, typename = std::enable_if_t<std::is_base_of_v<MPISlaveProcess, Slave>, void>>
class MPIProgram
{
public:
	MPIProgram(const void* data = nullptr, const std::size_t dataSize = 0, const MPI_Comm communicator = MPI_COMM_WORLD) : data_(data), dataSize_(dataSize), communicator_(communicator)
	{
		MPI_Init(nullptr, nullptr);
	}

	void run()
	{
		int rank {};
		
		MPI_Comm_rank(communicator_, &rank);
		if (rank == 0)
		{
			auto proc = Master(data_, dataSize_, communicator_);
			proc.run();
		}
		else
		{
			auto proc = Slave(data_, dataSize_, communicator_);
			proc.run();
		}

		MPI_Barrier(communicator_);
		MPI_Finalize();
	}

private:
	const void* data_;
	std::size_t dataSize_;
	MPI_Comm communicator_;
};
