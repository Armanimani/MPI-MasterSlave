#include "MPISlaveProcess.hpp"

MPISlaveProcess::MPISlaveProcess(const void* data, const std::size_t dataSize, const MPI_Comm communicator) : MPIProcess(communicator)
{
	MPI_Recv(nullptr, 0, MPI_INT, MASTER_ID_, static_cast<int>(MPITag::START_PROCESS), communicator_, MPI_STATUS_IGNORE);
	MPI_Send(nullptr, 0, MPI_INT, MASTER_ID_, static_cast<int>(MPITag::PROCESS_STARTED), communicator_);
}

void MPISlaveProcess::run()
{
	MPI_Send(nullptr, 0, MPI_INT, MASTER_ID_, static_cast<int>(MPITag::FREE), communicator_);
	while (true)
	{
		MPI_Status status;
		MPI_Probe(0, MPI_ANY_TAG, communicator_, &status);

		if (status.MPI_TAG == static_cast<int>(MPITag::TASK))
		{
			int count;
			MPI_Get_count(&status, MPI_CHAR, &count);
			char* buffer = new char[count];
			MPI_Recv(buffer, count, MPI_CHAR, MASTER_ID_, static_cast<int>(MPITag::TASK), communicator_, &status);
			std::string path = std::string(buffer);

			executeTask_(path);

			MPI_Send(nullptr, 0, MPI_INT, MASTER_ID_, static_cast<int>(MPITag::COMPLETED), communicator_);
			MPI_Send(nullptr, 0, MPI_INT, MASTER_ID_, static_cast<int>(MPITag::FREE), communicator_);

			delete[] buffer;
		}

		else if (status.MPI_TAG == static_cast<int>(MPITag::SHUTDOWN))
		{
			MPI_Recv(nullptr, 0, MPI_INT, MASTER_ID_, static_cast<int>(MPITag::SHUTDOWN), communicator_, &status);
			MPI_Send(nullptr, 0, MPI_INT, MASTER_ID_, static_cast<int>(MPITag::PROCESS_SHUTDOWN), communicator_);
			break;
		}
	}
}