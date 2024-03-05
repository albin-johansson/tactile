/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "directories.hpp"

#include <cstdlib>  // system

#include <centurion/filesystem.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "common/debug/assert.hpp"
#include "common/debug/panic.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

void open_directory(const Path& dir)
{
  if (fs::is_directory(dir)) {
    static const auto path = get_persistent_file_dir().string();
    if constexpr (kOnMacos) {
      static const auto cmd = fmt::format("open \"{}\"", path);
      std::system(cmd.c_str());
    }
    else if constexpr (kOnWindows) {
      static const auto cmd = fmt::format("explorer \"{}\"", path);
      std::system(cmd.c_str());
    }
    else if constexpr (kOnLinux) {
      static const auto cmd = fmt::format("xdg-open \"{}\"", path);
      std::system(cmd.c_str());
    }
    else {
      spdlog::warn("Cannot open file explorer on this platform!");
    }
  }
  else {
    throw TactileError {"Not a directory!"};
  }
}

auto find_resource(const char* resource) -> Path
{
  TACTILE_ASSERT(resource);
  static const Path dir {cen::base_path().copy()};
  return dir / resource;
}

auto get_widget_ini_path() -> const Path&
{
  static const auto ini = fs::absolute(find_resource("imgui.ini"));
  return ini;
}

auto get_persistent_file_dir() -> const Path&
{
  static const Path path {cen::preferred_path("albin-johansson", "tactile").copy()};
  return path;
}

}  // namespace tactile
