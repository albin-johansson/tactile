// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <cstdlib>    // EXIT_SUCCESS, EXIT_FAILURE
#include <exception>  // exception, set_terminate
#include <memory>     // make_unique
#include <utility>    // move

#include "tactile/base/prelude.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/terminate.hpp"
#include "tactile/core/engine/engine.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/log/terminal_log_sink.hpp"
#include "tactile/core/numeric/random.hpp"
#include "tactile/core/persist/protobuf_context.hpp"
#include "tactile/core/platform/sdl_context.hpp"
#include "tactile/core/platform/win32.hpp"
#include "tactile/core/tactile_editor.hpp"
#include "tactile/core/util/scope_guard.hpp"

#ifdef TACTILE_HAS_OPENGL_RENDERER
  #include "tactile/opengl/opengl_renderer.hpp"
#endif

auto main(const int, char*[]) -> int
{
  try {
    const auto startup_time = tactile::SteadyClock::now();
    std::set_terminate(&tactile::on_terminate);

    tactile::win32_enable_virtual_terminal_processing();

    auto terminal_sink = std::make_unique<tactile::TerminalLogSink>();
    terminal_sink->use_ansi_colors(true);

    tactile::Logger logger {};
    logger.set_reference_instant(startup_time);
    logger.set_min_level(tactile::LogLevel::kTrace);
    logger.add_sink(std::move(terminal_sink));

    tactile::set_default_logger(&logger);
    const tactile::ScopeGuard logger_guard {
      [] { tactile::set_default_logger(nullptr); }};

    const tactile::SetLogScope log_scope {"General"};

    TACTILE_LOG_INFO("Tactile " TACTILE_VERSION_STRING);

    tactile::init_random_number_generator();

    const tactile::ProtobufContext protobuf_context {};
    const tactile::SDLContext sdl_context {};

#ifdef TACTILE_HAS_OPENGL_RENDERER
    TACTILE_LOG_TRACE("Initializing OpenGL renderer");
    auto renderer = tactile::OpenGLRenderer::make();
#else
  #error "No renderer is available"
#endif

    if (!renderer.has_value()) {
      TACTILE_LOG_FATAL("Could not create renderer: {}",
                        renderer.error().message());
      return EXIT_FAILURE;
    }

    tactile::TactileEditor editor {renderer->get_window(), &renderer.value()};
    tactile::Engine engine {&editor, &renderer.value()};

    engine.run();

    return EXIT_SUCCESS;
  }
  catch (const tactile::Exception& exception) {
    TACTILE_LOG_FATAL("Unhandled exception: {}\n{}",
                      exception.what(),
                      exception.trace());
  }
  catch (const std::exception& exception) {
    TACTILE_LOG_FATAL("Unhandled exception: {}", exception.what());
  }
  catch (...) {
    TACTILE_LOG_FATAL("Unhandled exception");
  }

  return EXIT_FAILURE;
}
