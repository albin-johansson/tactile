// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <cstdlib>     // EXIT_SUCCESS, EXIT_FAILURE
#include <exception>   // exception, set_terminate
#include <filesystem>  // current_path, create_directories

#include <SDL2/SDL.h>

#include "tactile/core/editor_app.hpp"
#include "tactile/core/engine/engine.hpp"
#include "tactile/core/plugin/plugin_manager.hpp"
#include "tactile/foundation/debug/terminate_handler.hpp"
#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/foundation/log/logger_builder.hpp"
#include "tactile/foundation/math/rng.hpp"
#include "tactile/foundation/misc/scope_guard.hpp"
#include "tactile/foundation/platform/environment.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/render_context.hpp"
#include "tactile/core/cli/command_line_parser.hpp"

using namespace tactile;

auto main(const int argc, char* argv[]) -> int
{
  const auto startup_instant = tactile::SteadyClock::now();
  std::set_terminate(&tactile::on_terminate);

  try {
    const auto app_dir = std::filesystem::current_path();

    const auto cli_args = tactile::CommandLineParser::parse(argc, argv);
    if (cli_args.should_exit) {
      return EXIT_SUCCESS;
    }

    tactile::win32_enable_virtual_terminal_processing();

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

    const String requested_renderer_id {"tactile.opengl-renderer"};  // FIXME
    const auto plugin_manager = PluginManager::load(plugin_dir, requested_renderer_id);
    if (!plugin_manager.has_value()) {
      TACTILE_LOG_FATAL("Could not load plugins");
      return EXIT_FAILURE;
    }

    auto* window = RenderContext::get().get_window();
    if (!window) {
      TACTILE_LOG_FATAL("No window was installed");
      return EXIT_FAILURE;
    }

    auto* renderer = RenderContext::get().get_renderer();
    if (!renderer) {
      TACTILE_LOG_FATAL("No renderer was installed");
      return EXIT_FAILURE;
    }

    EditorApp app {window, renderer};
    Engine engine {&app, renderer};
    engine.run();

    return EXIT_SUCCESS;
  }
  catch (const Exception& err) {
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