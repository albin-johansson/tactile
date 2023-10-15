// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <cstdlib>     // EXIT_SUCCESS, EXIT_FAILURE
#include <exception>   // exception, set_terminate
#include <filesystem>  // current_path, create_directories

#include "tactile/core/debug/log/file_logger_sink.hpp"
#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/debug/log/terminal_logger_sink.hpp"
#include "tactile/core/debug/terminate_handler.hpp"
#include "tactile/core/io/save/save_format_manager.hpp"
#include "tactile/core/misc/scope_guard.hpp"
#include "tactile/core/platform/environment.hpp"
#include "tactile/core/plugin/plugin_manager.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

auto main(const int argc, char* argv[]) -> int
{
  (void) argc;
  (void) argv;

  const auto startup_instant = tactile::SteadyClock::now();
  std::set_terminate(&tactile::on_terminate);

  try {
    const auto app_dir = std::filesystem::current_path();

    tactile::win32_enable_virtual_terminal_processing();

    const auto log_dir = app_dir / "logs";  // FIXME
    std::filesystem::create_directories(log_dir);
    tactile::FileLoggerSink file_sink {log_dir / "log.txt"};

    tactile::TerminalLoggerSink console_sink;
    console_sink.use_ansi_colors(true);

    tactile::Logger logger;
    const tactile::ScopeGuard logger_guard {[] { tactile::set_default_logger(nullptr); }};

    logger.set_reference_instant(startup_instant);
    logger.set_min_level(tactile::LogLevel::kTrace);
    logger.add_sink(&file_sink);
    logger.add_sink(&console_sink);

    tactile::set_default_logger(&logger);

    TACTILE_LOG_DEBUG("Initialized logger");

    TACTILE_LOG_INFO("Tactile " TACTILE_VERSION_STRING);

    TACTILE_LOG_DEBUG("Loading plugins...");
    tactile::PluginManager plugin_manager;

    const auto plugin_dir = app_dir / "plugins";
    plugin_manager.scan(plugin_dir);

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