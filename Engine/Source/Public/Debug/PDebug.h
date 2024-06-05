#pragma once
#include <cstdint>
#include <iostream>
#include <string>

enum PELogType : uint8_t
{
	LT_LOG = 0U,
	LT_WARN,
	LT_ERROR,
	LT_SUCCESS
};

class PDebug
{
public:
	// Log a message to the console based on the log type
	static void Log(const std::string& message, const PELogType& logType = LT_LOG)
	{
		std::cout << message << std::endl;
	}
};