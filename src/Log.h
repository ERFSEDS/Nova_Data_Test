
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <memory>


namespace erfseds_nova {
	namespace test_data {
		
		class Log
		{
		public:
			static void Init(bool debug);
			static inline bool CanLog() { return s_Init && s_Logger.get(); }//Weather or not we have called Init yet
			static void Shutdown();

			inline static spdlog::logger* GetLogger() { return s_Logger.get(); }

		private:
			static std::unique_ptr<spdlog::logger> s_Logger;
			static volatile bool s_Init;
		};
	}
}

#define NOVA_TRACE(...)				::erfseds_nova::test_data::Log::GetLogger()->trace(__VA_ARGS__)
#define NOVA_INFO(...)				::erfseds_nova::test_data::Log::GetLogger()->info(__VA_ARGS__)
#define NOVA_WARN(...)				::erfseds_nova::test_data::Log::GetLogger()->warn(__VA_ARGS__)
#define NOVA_ERROR(...)				::erfseds_nova::test_data::Log::GetLogger()->error(__VA_ARGS__)
#define NOVA_CRITICAL(...)			::erfseds_nova::test_data::Log::GetLogger()->critical(__VA_ARGS__)
#define NOVA_LOG(level, ...)		::erfseds_nova::test_data::Log::GetLogger()->log(level, __VA_ARGS__)
