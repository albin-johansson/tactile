// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <cstdlib>     // EXIT_SUCCESS, EXIT_FAILURE
#include <exception>   // exception, set_terminate
#include <filesystem>  // current_path, create_directories

#include "tactile/core/debug/log/file_logger_sink.hpp"
#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/debug/log/terminal_logger_sink.hpp"
#include "tactile/core/debug/terminate_handler.hpp"
#include "tactile/core/format/file_format_manager.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

namespace {

inline constinit tactile::Logger gAppLogger;

}  // namespace

auto main(const int argc, char* argv[]) -> int
{
  (void) argc;
  (void) argv;

  const auto startup_instant = tactile::SteadyClock::now();
  std::set_terminate(&tactile::on_terminate);

  try {
    const auto app_dir = std::filesystem::current_path();

    const auto log_dir = app_dir / "logs";  // FIXME
    std::filesystem::create_directories(log_dir);
    tactile::FileLoggerSink file_sink {log_dir / "log.txt"};

    tactile::TerminalLoggerSink console_sink;
    console_sink.use_ansi_colors(true);

    gAppLogger.set_reference_instant(startup_instant);
    gAppLogger.set_min_level(tactile::LogLevel::kTrace);
    gAppLogger.add_sink(&file_sink);
    gAppLogger.add_sink(&console_sink);

    tactile::set_default_logger(&gAppLogger);

    TACTILE_LOG_DEBUG("Initialized logger");

    TACTILE_LOG_INFO("Tactile " TACTILE_VERSION_STRING);
  }
  catch (const tactile::Error& err) {
    TACTILE_LOG_FATAL("Unhandled exception: {}\n{}", err.what(), err.get_trace());
    return EXIT_FAILURE;
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_FATAL("Unhandled exception: {}", ex.what());
    return EXIT_FAILURE;
  }
  catch (...) {
    TACTILE_LOG_FATAL("Unhandled exception");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}