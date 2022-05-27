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

#include "preferences.hpp"

#include <filesystem>  // path, exists
#include <utility>     // move

#include <spdlog/spdlog.h>

#include "io/directories.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto get_file_path() -> const std::filesystem::path&
{
  static const auto path = io::persistent_file_dir() / "settings.bin";
  return path;
}

inline PreferenceState _settings;

}  // namespace

void load_preferences()
{
  _settings = PreferenceState{};

  const auto& path = get_file_path();
  if (std::filesystem::exists(path)) {
    _settings.parse(path);
  }
  else {
    spdlog::info("Did not find a preferences file, assuming defaults");
    save_preferences();
  }

  _settings.print();
}

void save_preferences()
{
  const auto& path = get_file_path();
  _settings.save(path);
}

void set_preferences(PreferenceState prefs)
{
  _settings = std::move(prefs);
}

auto get_preferences() -> PreferenceState&
{
  return _settings;
}

}  // namespace tactile
