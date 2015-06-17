#include "Logger.h"

namespace Silcoish
{
	namespace Utils
	{
		Logger::Logger(const char *fileName, bool printToConsole, bool clear)
		{
			print = printToConsole;

			if (!clear)
				logFile.open(fileName, std::ofstream::out | std::ofstream::app);
			else
				logFile.open(fileName, std::ofstream::out | std::ofstream::trunc);
			
			// Write beginning of log
			if (logFile.is_open())
			{
				start = std::chrono::system_clock::now();
				std::time_t time = std::chrono::system_clock::to_time_t(start);

				logFile << "Logging started " << std::ctime(&time) << std::endl;
			}
		}

		Logger::~Logger()
		{
			//Write end of log then close
			if (logFile.is_open())
			{
				end = std::chrono::system_clock::now();
				std::time_t time = std::chrono::system_clock::to_time_t(end);
				std::chrono::duration<double> elapsedSeconds = end-start;
				logFile << "Logging open for: " << elapsedSeconds.count() << " seconds" << std::endl;
				logFile << "Logging finished " << std::ctime(&time) << std::endl;
				logFile << "------------------------------------------------" << std::endl << std::endl;
				logFile.flush();
				logFile.close();
			}
		}

		bool Logger::WriteToFile(Logger &logger, const char *chars)
		{
			return false;
		}

		//overload the << operator with a log type. Logs to file specific log type.
		Logger &operator << (Logger &logger, const Logger::logType type)
		{
			switch (type)
			{
			case Logger::logType::T_ERROR:
				logger.logFile << "[ERROR]: ";
				break;
			case Logger::logType::T_WARNING:
				logger.logFile << "[WARNING]: ";
				break;
			default:
				break;
			}

			return logger;
		}

		//overload the << operator with the chars. Logs to chars to file.
		Logger &operator << (Logger &logger, const char *chars)
		{
			logger.logFile << chars << std::endl;
			return logger;
		}

		//overload the << operator with then int. Logs the int to file.
		Logger &operator << (Logger &logger, const unsigned int i)
		{
			logger.logFile << i << std::endl;
			return logger;
		}
	}
}