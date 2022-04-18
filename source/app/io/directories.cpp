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

#include "directories.hpp"

#include <centurion/filesystem.hpp>

#include "misc/assert.hpp"

namespace tactile {

auto find_resource(const char* resource) -> std::filesystem::path
{
  TACTILE_ASSERT(resource);
  static const std::filesystem::path dir{cen::base_path().copy()};
  return dir / resource;
}

auto widget_ini_path() -> const std::filesystem::path&
{
  static const auto ini = absolute(find_resource("imgui.ini"));
  return ini;
}

auto persistent_file_dir() -> const std::filesystem::path&
{
  static const std::filesystem::path path{
      cen::preferred_path("albin-johansson", "tactile").copy()};
  return path;
}

}  // namespace tactile
