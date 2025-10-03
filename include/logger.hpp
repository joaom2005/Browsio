#ifndef BROWSIO_LOG_H
#define BROWSIO_LOG_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <memory>
#include <iostream>

class Logger {
public:
    enum class Level { Info, Warn, Error };

    // Initialize logger (call once at program start)
    static void Init(const std::string& logFile = "logs/app.log") {
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
            s_logger->set_level(spdlog::level::info);
            s_logger->flush_on(spdlog::level::info);

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
};

#endif