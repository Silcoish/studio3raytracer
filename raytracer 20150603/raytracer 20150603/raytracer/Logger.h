#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

namespace Silcoish
{
	namespace Utils
	{
		class Logger
		{
		private:
			std::ofstream logFile;
			std::chrono::time_point<std::chrono::system_clock>start, end;
			bool print = false;
		public:
			enum class logType
			{
				T_WARNING,
				T_ERROR
			};
		private:
			bool WriteToFile(Logger &logger, const char *chars);
		public:
			Logger(const char *fileName = "defaultname.log", bool printToConsole = false, bool clear = false);
			~Logger();
			friend Logger &operator << (Logger &logger, const logType);
			friend Logger &operator << (Logger &logger, const char *chars);
			friend Logger &operator << (Logger &logger, const unsigned int i);
		};
	}
}

#endif