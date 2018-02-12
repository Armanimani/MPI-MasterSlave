#include "IntegralSlave.hpp"

#include <fstream>
#include <cmath>

const int SUB_DIVISION = 10000000;

IntegralSlave::IntegralSlave(const void* data, const std::size_t dataSize, const MPI_Comm communicator) : MPISlaveProcess(data, dataSize, communicator)
{
	// Slave initialization with "data" goes here
}

void IntegralSlave::executeTask_(const std::string& path)
{
	std::ifstream file(path);

	double begin {};
	double end {};

	file >> begin;
	file >> end;

	double x = begin;
	double dx = (end - begin) / SUB_DIVISION;

	double sum {0};
	while (x <= end)
	{
		sum += sqrt(1 - x * x) * dx;
		x += dx;
	}
	auto pos = path.find_last_of('/');
	auto outputPath = std::string(path.begin(), path.begin() + pos) + "/output.dat";
	
	std::ofstream output(outputPath);
	output << sum;
}