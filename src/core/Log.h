#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Chipbit {

  class Log
  {
  public:
    static std::shared_ptr<spdlog::logger> Get() {
      return GetInstance().m_Logger;
    }
  private:
    static Log& GetInstance() {
      static Log instance;

      if(instance.m_Logger == nullptr) {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Chipbit.log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        instance.m_Logger = std::make_shared<spdlog::logger>("Chipbit", begin(logSinks), end(logSinks));

        spdlog::register_logger(instance.m_Logger);

        instance.m_Logger->set_level(spdlog::level::trace);
        instance.m_Logger->flush_on(spdlog::level::trace);
      }

      return instance;
    }
  private:
    std::shared_ptr<spdlog::logger> m_Logger = nullptr;
  };
}

// Core log macros
#define CB_TRACE(...)    ::Chipbit::Log::Get()->trace(__VA_ARGS__)
#define CB_INFO(...)     ::Chipbit::Log::Get()->info(__VA_ARGS__)
#define CB_WARN(...)     ::Chipbit::Log::Get()->warn(__VA_ARGS__)
#define CB_ERROR(...)    ::Chipbit::Log::Get()->error(__VA_ARGS__)
#define CB_CRITICAL(...) ::Chipbit::Log::Get()->critical(__VA_ARGS__)