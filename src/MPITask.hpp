#pragma once

#include <memory>
#include <string>

// The data for the tasks need to be written to a file and the file path need to be saved in "path" variable
struct MPITask
{
	MPITask(const std::string m_path) : path(std::move(m_path)) {}
	std::string path;
};