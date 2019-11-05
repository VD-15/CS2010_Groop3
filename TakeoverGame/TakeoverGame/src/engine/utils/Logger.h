#pragma once
#include <fstream>
#include <string>
#include <queue>
#include <tuple>

#include <thread>
#include <condition_variable>

#include "../core/ValkyrieEngine.h"

namespace vlk
{
	const extern UInt VLK_LOG_LEVEL_TRACE;
	const extern UInt VLK_LOG_LEVEL_VERBOSE;
	const extern UInt VLK_LOG_LEVEL_INFO;
	const extern UInt VLK_LOG_LEVEL_WARNING;
	const extern UInt VLK_LOG_LEVEL_ERROR;
	const extern UInt VLK_LOG_LEVEL_FATAL;
	const extern UInt VLK_LOG_LEVEL_ANNOUNCE;

	class Logger
	{
		public:
		Logger();
		virtual ~Logger();

		void EnableSecondaryOutput();
		void DisableSecondaryOutput();

		void Open(std::string path);
		void Close();

		void SetLogLevel(UInt level);
		void Log(const std::string& source, UInt level, const std::string& message);

		UInt GetLogLevel() const;

		private:
		std::ofstream _output;
		UInt _logLevel;
		bool _doSecondaryOutput;
	};

	std::string LogLevelToString(UInt level);
}
