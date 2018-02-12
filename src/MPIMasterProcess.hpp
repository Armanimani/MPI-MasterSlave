#pragma once

#include "MPIProcess.hpp"
#include "MPITask.hpp"

#include <vector>
#include <memory>

class MPIMasterProcess : public MPIProcess
{
public:
	MPIMasterProcess(const void* data = nullptr, const std::size_t dataSize = 0, const MPI_Comm communicator = MPI_COMM_WORLD);
	void run();

protected:
	std::vector<std::unique_ptr<MPITask>> tasks_;

	virtual void createTasks_() = 0; // This pupulates the "tasks_" array. The data for the tasks need to be written to a file and the file path need to be saved in "path" variable
	virtual void postProcessing_();

private:
	void shutdownProcess_(const int id);
	void sendTask_(const int taskID, const int destination);
};