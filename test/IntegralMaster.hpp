#pragma once

#include "../src/MPIMasterProcess.hpp"

class IntegralMaster : public MPIMasterProcess
{
public:
	IntegralMaster(const void* data = nullptr, const std::size_t dataSize = 0, const MPI_Comm communicator = MPI_COMM_WORLD);

protected:
	void createTasks_() override;
	void postProcessing_() override;
};
