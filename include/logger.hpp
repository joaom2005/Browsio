#ifndef BROWSIO_LOG_H
#define BROWSIO_LOG_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

class Logger {
public:
    enum class Level { Info, Warn, Error };

    // Initialize logger (call once at program start)
    static void Init(std::string logFile = "", Level logLevel = Level::Info) {
        if (logFile.empty()){
            logFile = GetFilenameByCurrentDate();
        }

        spdlog::level::level_enum spdLogLevel;
        switch (logLevel)
        {
        case Level::Info:
            spdLogLevel = spdlog::level::info;
            break;
        case Level::Warn:
            spdLogLevel = spdlog::level::warn;
            break;
        case Level::Error:
            spdLogLevel = spdlog::level::err;
            break;
        default:
            spdLogLevel = spdlog::level::info;
            break;
        }

        try {
            // Console sink
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_pattern("[%T] [%^%l%$] %v");

            // Rotating file sink: 5 MB per file, keep 3 files
            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFile, 5 * 1024 * 1024, 3);
            file_sink->set_pattern("[%Y-%m-%d %T] [%l] %v");

            // Combine sinks
            s_logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{console_sink, file_sink});
            spdlog::register_logger(s_logger);
            s_logger->set_level(spdLogLevel);
            s_logger->flush_on(spdLogLevel);

            Info("Logger initialized!");
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
        }
    }

    // Logging functions
    template<typename... Args>
    static void Info(const std::string& msg, Args&&... args) {
        s_logger->info(msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warn(const std::string& msg, Args&&... args) {
        s_logger->warn(msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(const std::string& msg, Args&&... args) {
        s_logger->error(msg, std::forward<Args>(args)...);
    }

    static void SetLevel(Level level) {
        switch(level) {
            case Level::Info: s_logger->set_level(spdlog::level::info); break;
            case Level::Warn: s_logger->set_level(spdlog::level::warn); break;
            case Level::Error: s_logger->set_level(spdlog::level::err); break;
        }
    }

private:
    inline static std::shared_ptr<spdlog::logger> s_logger;

    // Simple function to get the current time and use it as the log's name
    inline static std::string GetFilenameByCurrentDate(const char* extension = ".log") {
        auto now = std::chrono::system_clock::now();
        auto localTime = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << "logs/" << std::put_time(std::localtime(&localTime), "%Y-%m-%d-%H-%M-%S") << extension;
        return ss.str();
    }
};

// Use Macros so that, when compiling in release mode, no extra code will be included
#ifndef NDEBUG

#define LOG_INIT(LOG_FILE, LEVEL) Logger::Init(LOG_FILE, LEVEL);

#define SET_LOG_LEVEL(LEVEL) Logger::SetLevel(LEVEL);

#define LOG_INFO(...)  Logger::Info(__VA_ARGS__)
#define LOG_WARN(...)  Logger::Warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::Error(__VA_ARGS__)

#else

#define LOG_INIT

#define SET_LOG_LEVEL(LEVEL)

#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)

#endif

#endif