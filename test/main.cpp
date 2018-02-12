#include "IntegralSlave.hpp"
#include "IntegralMaster.hpp"
#include "../src/MPIProgram.hpp"

int main()
{
	MPIProgram<IntegralMaster, IntegralSlave> program;
	program.run();
	return 0;
}