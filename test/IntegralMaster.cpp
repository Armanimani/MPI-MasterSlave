#include "IntegralMaster.hpp"

#include <fstream>
#include <iostream>

const int NUMBER_OF_DIVISIONS = 500;

IntegralMaster::IntegralMaster(const void* data, const std::size_t dataSize, const MPI_Comm communicator) : MPIMasterProcess(data, dataSize, communicator) 
{
	// additional initialization with "data" goes here
}

void IntegralMaster::createTasks_()
{
	std::system("rm -rf temp");
	std::system("mkdir temp");

	double dx = 1.0 / NUMBER_OF_DIVISIONS;
	for (auto i = 0; i != NUMBER_OF_DIVISIONS; ++i)
	{
		double start = 0 + i * dx; // starting point of integral
		double end = 0 + (i + 1) * dx;	// end point of integral

		std::string folderPath = std::string("temp/" + std::to_string(i + 1));
		std::system(std::string("mkdir " + folderPath).c_str());
		
		std::string filePath = folderPath + "/task.dat";
		std::ofstream file(filePath);

		file << start << '\n' << end;

		tasks_.push_back(std::make_unique<MPITask>(filePath));
	}
}

void IntegralMaster::postProcessing_()
{
	double sum {0};
	for (auto i = 0; i != NUMBER_OF_DIVISIONS; ++i)
	{
		std::string path = "temp/" + std::to_string(i + 1) + "/output.dat";
		std::ifstream file(path);

		double value {0};
		file >> value;

		sum += value;
	}

	std::cout << "**********************************\n The sum = " << sum * 4 << "\n**********************************" << std::endl; 
}