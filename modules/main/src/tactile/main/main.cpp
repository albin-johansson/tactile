// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <cstdlib>     // EXIT_SUCCESS, EXIT_FAILURE
#include <exception>   // exception, set_terminate
#include <filesystem>  // current_path, create_directories

#include <SDL2/SDL.h>

#include "tactile/core/debug/log/logger_builder.hpp"
#include "tactile/core/debug/terminate_handler.hpp"
#include "tactile/core/io/save/save_format_manager.hpp"
#include "tactile/core/math/rng.hpp"
#include "tactile/core/misc/scope_guard.hpp"
#include "tactile/core/platform/environment.hpp"
#include "tactile/core/plugin/plugin_manager.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/render/render_context.hpp"

using namespace tactile;

auto main(const int argc, char* argv[]) -> int
{
  (void) argc;
  (void) argv;

  std::set_terminate(&on_terminate);

  try {
    const auto app_dir = std::filesystem::current_path();
    const auto log_dir = app_dir / "logs";  // FIXME
    const auto plugin_dir = app_dir / "plugins";

    std::filesystem::create_directories(log_dir);

    auto logger = LoggerBuilder {}
                      .use_initialization_time_as_reference_instant()
                      .min_level(LogLevel::kTrace)
                      .flush_on(LogLevel::kError)
                      .with_file_sink(log_dir / "log.txt")
                      .with_terminal_sink()
                      .use_colored_terminal_output()
                      .build();

    const ScopeGuard logger_guard {[] { set_default_logger(nullptr); }};

    set_default_logger(&logger);
    win32_enable_virtual_terminal_processing();

    TACTILE_LOG_DEBUG("Initialized logger");

    TACTILE_LOG_INFO("Tactile " TACTILE_VERSION_STRING);

    rng_init();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      TACTILE_LOG_FATAL("Could not initialize SDL: {}", SDL_GetError());
      return EXIT_FAILURE;
    }

    const ScopeGuard sdl_guard {[] { SDL_Quit(); }};

    SDL_SetHint(SDL_HINT_APP_NAME, "Tactile");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    TACTILE_LOG_DEBUG("Loading plugins...");

    PluginManager plugin_manager;
    plugin_manager.scan(plugin_dir);

    for (const auto& plugin_data : plugin_manager.get_plugins()) {
      plugin_data.plugin->on_load();  // TODO don't load all plugins automatically
    }

    const ScopeGuard plugin_guard {[&] {
      TACTILE_LOG_DEBUG("Unloading plugins...");
      for (const auto& plugin_data : plugin_manager.get_plugins()) {
        plugin_data.plugin->on_unload();
      }
    }};

    auto& render_context = RenderContext::get();

    if (!render_context.get_window()) {
      TACTILE_LOG_FATAL("No window was installed");
      return EXIT_FAILURE;
    }

    if (!render_context.get_renderer()) {
      TACTILE_LOG_FATAL("No renderer was installed");
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }
  catch (const Error& err) {
    TACTILE_LOG_FATAL("Unhandled exception: {}\n{}", err.what(), err.get_trace());
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_FATAL("Unhandled exception: {}", ex.what());
  }
  catch (...) {
    TACTILE_LOG_FATAL("Unhandled exception");
  }

  return EXIT_FAILURE;
}