#include "Logger.h"
#include <iostream>
#include <ctime>
#include <chrono>

std::vector<LogEntry> Logger::m_messages;

void Logger::Log(const std::string& msg)
{
	LogEntry entry;
	entry.type = LOG_INFO;
	entry.message = "LOG: [" + CurrentDateTimeToString() + "] " + msg;
	std::cout << "\033[1;32m" << entry.message << "\033[0m\n";
	m_messages.push_back(entry);
}

void Logger::Err(const std::string& msg)
{
	LogEntry entry;
	entry.type = LOG_ERROR;
	entry.message = "ERROR: [" + CurrentDateTimeToString() + "] " + msg;
	std::cerr << "\033[1;31m" << entry.message << "\033[0m\n";
	m_messages.push_back(entry);
}

std::string Logger::CurrentDateTimeToString()
{
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	std::tm timeinfo;
	localtime_s(&timeinfo, &now);
	std::strftime(&output[0], output.size(), "%D %T", &timeinfo);

	return output;
}
