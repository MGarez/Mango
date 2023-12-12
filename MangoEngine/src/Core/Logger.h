#pragma once
#include <string>
#include <vector>

// TODO: Use a profesional logger library:
// spdlog (modern c++)
// log.c (base c)

enum LogType
{
	LOG_INFO,
	LOG_WARNING, 
	LOG_ERROR
};

struct LogEntry
{
	LogType type;
	std::string message;
};

class Logger
{ 
public:

	static void Log(const std::string& msg);
	static void Err(const std::string& msg);

private:
	// Todo: move this to a generic time function
	static std::string CurrentDateTimeToString();

private:

	static std::vector<LogEntry> m_messages;

};

