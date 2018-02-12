#include "MPIMasterProcess.hpp"

#include <mpi.h>
#include <iostream>

MPIMasterProcess::MPIMasterProcess(const void* data, const std::size_t dataSize, const MPI_Comm communicator) : MPIProcess(communicator)
{
	std::cout << "\n***INFO*** -> Starting MPI with " << size_ << " process...\n";
	std::cout << "\tProcess " << rank_ << " started\n";

	std::vector<MPI_Request> requests(size_ - 1);
	for (int i = 1; i != size_; ++i)
	{
		MPI_Send(nullptr, 0, MPI_INT, i, static_cast<int>(MPITag::START_PROCESS), communicator_);
		MPI_Irecv(nullptr, 0, MPI_INT, i, static_cast<int>(MPITag::PROCESS_STARTED), communicator_,&requests[i - 1]);
		std::cout << "\tProcess " << i << " started\n";
	}
	MPI_Waitall(size_ - 1, &requests[0], MPI_STATUSES_IGNORE);
	std::cout << "\n";
}


void MPIMasterProcess::run()
{
	createTasks_();

	int taskCounter {0};
	int runningSlaveProcess {size_ - 1};
	int completedTasks {0};
	std::vector<int> runningTasks(size_ - 1);

	while (true)
	{
		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, communicator_, &status);
		
		if (status.MPI_TAG == static_cast<int>(MPITag::FREE))
		{
			auto source = status.MPI_SOURCE;
			MPI_Recv(nullptr, 0, MPI_INT, source, static_cast<int>(MPITag::FREE), communicator_, MPI_STATUS_IGNORE);

			if (taskCounter == tasks_.size()) 
			{
				shutdownProcess_(source);
				runningSlaveProcess--;
			}
			else
			{
				taskCounter++;
				sendTask_(taskCounter, source);
				runningTasks[source - 1] = taskCounter;
			}
		}
		else if (status.MPI_TAG == static_cast<int>(MPITag::COMPLETED))
		{
			auto source = status.MPI_SOURCE;
			MPI_Recv(nullptr, 0, MPI_INT, source, static_cast<int>(MPITag::COMPLETED), communicator_, MPI_STATUS_IGNORE);
			std::cout << "\t\tProcess " << source << " finished task " << runningTasks[source - 1] << std::endl;
			completedTasks++;
		}
		if (runningSlaveProcess == 0)
		{
			break;
		}
	}

	postProcessing_();

	std::cout << "\tProcess 0 is shutting down..." << std::endl; 
}

void MPIMasterProcess::shutdownProcess_(const int id)
{
	MPI_Send(nullptr, 0, MPI_INT, id, static_cast<int>(MPITag::SHUTDOWN), communicator_);
	MPI_Recv(nullptr, 0, MPI_INT, id, static_cast<int>(MPITag::PROCESS_SHUTDOWN), communicator_, MPI_STATUS_IGNORE);

	std::cout << "\tProcess " << id << " is shutting down..." << std::endl; 
}

void MPIMasterProcess::sendTask_(const int taskID, const int destination)
{
	std::cout << "\tSending task " << taskID << "/" << tasks_.size() << " to process " << destination << "..." << std::endl;
	MPI_Send(tasks_[taskID - 1]->path.c_str(), tasks_[taskID - 1]->path.size() + 1, MPI_CHAR, destination, static_cast<int>(MPITag::TASK), communicator_);
}

void MPIMasterProcess::postProcessing_()
{
	// how to handle the data that are written by slave processs to disk after the computation is done
}