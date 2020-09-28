#include "Log.h"

#include <mutex>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define LOG_USE_FILES 1
#define LOG_NAME "Nova Test Data Generator"


namespace erfseds_nova {
	namespace test_data {

		volatile bool Log::s_Init = false;
		std::mutex s_ConstructMutex;
		
		std::unique_ptr<spdlog::logger> Log::s_Logger(nullptr);

		void Log::Init(bool debug)
		{
			std::lock_guard<std::mutex> guard(s_ConstructMutex);

			if (s_Init) return;

			bool useFiles = true;
			std::string consolePattern = "%^[%T] %n: %$%v", filePattern = "%n-%t:[%D %H:%M:%S.%e] %l: %v";


			auto stdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			stdOut->set_pattern(consolePattern);


#if LOG_USE_FILES
			auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("nova-data-generator-log.txt", true);
			file->set_pattern(filePattern);
			file->set_level(spdlog::level::level_enum::trace);

			s_Logger.reset(new spdlog::logger(LOG_NAME, { stdOut, file }));
#else
			s_Logger.reset(new spdlog::logger(LOG_NAME, { stdOut }));

#endif

			s_Logger->set_level(spdlog::level::level_enum::trace);

			if (debug)
			{
				stdOut->set_level(spdlog::level::level_enum::trace);
			}
			else
			{
				stdOut->set_level(spdlog::level::level_enum::info);
			}

			Log::s_Init = true;
			NOVA_TRACE("Logging Initalized");
		}

		void Log::Shutdown()
		{
			std::lock_guard<std::mutex> guard(s_ConstructMutex);

			s_Init = false;
			NOVA_TRACE("Destroying logging");
			s_Logger.reset(nullptr);
			
		}
	}
}
