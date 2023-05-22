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

#include "file_history.hpp"

#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/numeric.hpp"
#include "common/type/maybe.hpp"
#include "common/util/filesystem.hpp"

namespace tactile {
namespace {

constexpr usize kHistoryMaxSize = 10;

inline FileHistory gHistory;

}  // namespace

void clear_file_history()
{
  spdlog::debug("[History] Clearing file history");
  gHistory.entries.clear();
}

void add_to_file_history(const Path& path)
{
  auto converted = use_forward_slashes(path);
  if (std::find(gHistory.entries.begin(), gHistory.entries.end(), converted) ==
      gHistory.entries.end()) {
    spdlog::debug("[History] Storing '{}'", converted);
    gHistory.entries.push_back(std::move(converted));

    if (gHistory.entries.size() > kHistoryMaxSize) {
      gHistory.entries.pop_front();
    }
  }
  else {
    spdlog::debug("[History] Did not add existing entry '{}'", converted);
  }
}

void set_last_closed_file(const Path& path)
{
  gHistory.last_closed_file = use_forward_slashes(path);
  spdlog::debug("[History] Last closed file is now '{}'", *gHistory.last_closed_file);

  add_to_file_history(path);
}

auto get_last_closed_file() -> const String&
{
  if (is_last_closed_file_valid()) {
    return gHistory.last_closed_file.value();
  }
  else {
    throw TactileError {"Invalid last closed file"};
  }
}

auto is_last_closed_file_valid() -> bool
{
  return gHistory.last_closed_file && fs::exists(Path {*gHistory.last_closed_file});
}

void set_file_history(FileHistory history)
{
  gHistory = std::move(history);
}

auto get_file_history() -> const FileHistory&
{
  return gHistory;
}

}  // namespace tactile
