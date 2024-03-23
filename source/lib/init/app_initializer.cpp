// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "app_initializer.hpp"

#include <cstdlib>    // abort
#include <exception>  // exception, set_terminate

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/logging.hpp"
#include "common/util/fmt.hpp"
#include "io/proto/settings.hpp"
#include "lang/language.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/core/debug/stacktrace.hpp"
#include "tactile/core/platform/filesystem.hpp"
#include "tactile/core/platform/win32.hpp"
#include "ui/menu/menu.hpp"

namespace tactile {

AppInitializer::AppInitializer()
{
  std::set_terminate(&on_terminate);

  // Configure custom log sinks and output format
  init_logger();

  // Initialize the Protobuf library
  mProtobuf.emplace();

  // Initialize SDL and the application window
  mSDL.emplace();
  mRenderer.emplace(OpenGLRenderer::make().value());

  // win32_use_immersive_dark_mode(window.get());

  // Configure settings and load the language files
  get_settings().copy_values_from(load_settings_from_disk());
  get_settings().print();

  load_languages();

  // Initialize Dear ImGui and other UI-related state
  mImGui.emplace();
  init_menus();

  spdlog::debug("Using persistent file directory {}",
                get_persistent_storage_directory().value());
}

auto AppInitializer::get_window() -> IWindow*
{
  return mRenderer->get_window();
}

void on_terminate()
{
  try {
    spdlog::critical("Into exile I must go. Failed I have.\n{}", get_stacktrace());
  }
  catch (...) {
    // Not much we can do.
  }

  std::abort();
}

}  // namespace tactile
