#pragma once

#include "MPIProcess.hpp"

#include <string>

class MPISlaveProcess : public MPIProcess
{
public:
	MPISlaveProcess(const void* data = nullptr, const std::size_t dataSize = 0, const MPI_Comm communicator = MPI_COMM_WORLD);
	void run();

protected:
	virtual void executeTask_(const std::string& path) = 0;
};