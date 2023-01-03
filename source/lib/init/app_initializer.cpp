/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "app_initializer.hpp"

#include <cstdlib>    // abort
#include <exception>  // exception, set_terminate

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/logging.hpp"
#include "common/debug/stacktrace.hpp"
#include "common/type/path.hpp"
#include "common/util/fmt.hpp"
#include "io/directories.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "system/win32.hpp"
#include "ui/menu/menu.hpp"

namespace tactile {
namespace {

void load_window_icon(cen::window& window)
{
  try {
    // This is necessary to allow macOS builds in different flavours
    const auto icon_path = io::find_resource(is_app_bundle ? "Tactile.icns"  //
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
  win32_use_immersive_dark_mode(window);
  load_window_icon(window);

  // Configure settings and load the language files
  io::load_preferences();
  load_languages();

  // Initialize Dear ImGui and other UI-related state
  mImGui.emplace(window, sdl.get_gl_context());
  init_menus();

  spdlog::debug("Using persistent file directory {}", tactile::io::persistent_file_dir());
}

auto AppInitializer::get_window() -> cen::window&
{
  return mSDL.value().get_window();
}

void on_terminate()
{
  try {
    spdlog::critical("Into exile I must go. Failed I have.\n{}",
                     boost::stacktrace::stacktrace {});
  }
  catch (...) {
    // Not much we can do.
  }

  std::abort();
}

}  // namespace tactile