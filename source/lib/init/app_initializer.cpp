// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "app_initializer.hpp"

#include <cstdlib>    // abort
#include <exception>  // exception, set_terminate

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/logging.hpp"
#include "common/util/fmt.hpp"
#include "io/directories.hpp"
#include "io/proto/settings.hpp"
#include "lang/language.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/core/debug/stacktrace.hpp"
#include "tactile/core/platform/win32.hpp"
#include "ui/menu/menu.hpp"

namespace tactile {
namespace {

void load_window_icon(cen::window& window)
{
  try {
    // This is necessary to allow macOS builds in different flavours
    const auto icon_path = find_resource(kIsAppBundle ? "Tactile.icns"  //
                                                      : "assets/icon.png");
    window.set_icon(cen::surface {icon_path.string()});
  }
  catch (const std::exception& e) {
    spdlog::error("Failed to load window icon: {}", e.what() ? e.what() : "N/A");
  }
}

}  // namespace

AppInitializer::AppInitializer()
{
  std::set_terminate(&on_terminate);

  // Configure custom log sinks and output format
  init_logger();

  // Initialize the Protobuf library
  mProtobuf.emplace();

  // Initialize SDL and the application window
  auto& sdl = mSDL.emplace();
  auto& window = sdl.get_window();
  win32_use_immersive_dark_mode(window.get());
  load_window_icon(window);

  // Configure settings and load the language files
  get_settings().copy_values_from(load_settings_from_disk());
  get_settings().print();

  load_languages();

  // Initialize Dear ImGui and other UI-related state
  mImGui.emplace(window, sdl.get_gl_context());
  init_menus();

  spdlog::debug("Using persistent file directory {}", tactile::get_persistent_file_dir());
}

auto AppInitializer::get_window() -> cen::window&
{
  return mSDL.value().get_window();
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
