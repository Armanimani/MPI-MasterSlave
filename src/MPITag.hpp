#pragma once

enum class MPITag : int
{
	FREE = 0,
	COMPLETED = 1,
	SHUTDOWN = 2,
	PROCESS_SHUTDOWN = 3,
	TASK = 4,
	START_PROCESS = 5,
	PROCESS_STARTED = 6,
};
