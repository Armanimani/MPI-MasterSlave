#pragma once

#include "../src/MPISlaveProcess.hpp"

class IntegralSlave : public MPISlaveProcess
{
public:
	IntegralSlave(const void* data = nullptr, const std::size_t dataSize = 0, const MPI_Comm communicator = MPI_COMM_WORLD);

protected:
	void executeTask_(const std::string& path) override;
};