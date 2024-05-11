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
#include "tactile/core/platform/dynamic_library.hpp"
#include "tactile/core/platform/sdl_context.hpp"
#include "tactile/core/platform/win32.hpp"
#include "tactile/core/tactile_editor.hpp"
#include "tactile/core/util/scope_guard.hpp"
#include "tactile/render/renderer.hpp"

namespace tactile {

struct RendererFunctions final
{
  using make_renderer_t = IRenderer*();
  using free_renderer_t = void(IRenderer*);

  Unique<IDynamicLibrary> lib;
  make_renderer_t* make_renderer;
  free_renderer_t* free_renderer;
};

using UniqueRenderer = Unique<IRenderer, RendererFunctions::free_renderer_t*>;

[[nodiscard]]
auto _load_renderer_functions(const Path& libpath) -> Maybe<RendererFunctions>
{
  RendererFunctions functions {};

  functions.lib = load_library(libpath);
  if (!functions.lib) {
    TACTILE_LOG_ERROR("Could not load renderer library");
    return kNone;
  }

  functions.make_renderer =
      find_symbol<RendererFunctions::make_renderer_t>(*functions.lib,
                                                      "tactile_make_renderer");
  functions.free_renderer =
      find_symbol<RendererFunctions::free_renderer_t>(*functions.lib,
                                                      "tactile_free_renderer");

  if (!functions.make_renderer || !functions.free_renderer) {
    TACTILE_LOG_ERROR("Could not load renderer functions");
    return kNone;
  }

  return functions;
}

[[nodiscard]]
auto _make_logger() -> Logger
{
  win32_enable_virtual_terminal_processing();

  auto terminal_sink = std::make_unique<TerminalLogSink>();
  terminal_sink->use_ansi_colors(true);

  Logger logger {};

  logger.set_reference_instant(SteadyClock::now());
  logger.set_min_level(LogLevel::kTrace);
  logger.add_sink(std::move(terminal_sink));

  return logger;
}

[[nodiscard]]
auto _run() -> int
{
  std::set_terminate(&on_terminate);

  auto logger = _make_logger();
  set_default_logger(&logger);
  const ScopeGuard logger_guard {[] { set_default_logger(nullptr); }};
  const SetLogScope log_scope {"General"};

  TACTILE_LOG_INFO("Tactile " TACTILE_VERSION_STRING);

  init_random_number_generator();

  const ProtobufContext protobuf_context {};
  const SDLContext sdl_context {};

  const auto renderer_functions =
      _load_renderer_functions("tactile-opengl" TACTILE_DLL_EXT);
  if (!renderer_functions.has_value()) {
    return EXIT_FAILURE;
  }

  UniqueRenderer renderer {renderer_functions->make_renderer(),
                           renderer_functions->free_renderer};
  if (!renderer) {
    TACTILE_LOG_ERROR("Could not create renderer");
    return EXIT_FAILURE;
  }

  TactileEditor editor {renderer->get_window(), renderer.get()};

  Engine engine {&editor, renderer.get()};
  engine.run();

  return EXIT_SUCCESS;
}

}  // namespace tactile

auto main(const int, char*[]) -> int
{
  try {
    return tactile::_run();
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
