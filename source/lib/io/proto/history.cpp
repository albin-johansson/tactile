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

#include "history.hpp"

#include <algorithm>  // find
#include <utility>    // move

#include <spdlog/spdlog.h>

#include "common/type/maybe.hpp"
#include "common/util/filesystem.hpp"
#include "common/util/fmt.hpp"
#include "core/debug/panic.hpp"
#include "core/vocabulary.hpp"
#include "io/directories.hpp"
#include "io/file.hpp"
#include "io/proto/proto.hpp"

namespace tactile::io {
namespace {

constexpr int history_format_version [[maybe_unused]] = 1;
constexpr usize history_max_size = 10;

// We store paths as strings because that makes displaying them
// in menus much easier (and faster)
inline Maybe<String> history_last_closed_file;
inline Deque<String> history_entries;

[[nodiscard]] auto get_file_path() -> const Path&
{
  static const auto path = persistent_file_dir() / "history.bin";
  return path;
}

}  // namespace

void load_file_history()
{
  spdlog::debug("Loading file history...");
  auto stream = read_file(get_file_path(), FileType::Binary);

  proto::History h;
  if (h.ParseFromIstream(&stream)) {
    if (h.has_last_opened_file()) {
      history_last_closed_file = h.last_opened_file();
    }

    for (const auto& file: h.files()) {
      if (fs::exists(file)) {
        spdlog::debug("Loaded '{}' from file history", file);
        history_entries.push_back(file);
      }
    }
  }
  else {
    spdlog::warn("Failed to read history file (this is expected for first time runs)");
  }
}

void save_file_history()
{
  proto::History h;

  if (history_last_closed_file) {
    h.set_last_opened_file(*history_last_closed_file);
  }

  for (const auto& path: history_entries) {
    spdlog::debug("Saving '{}' to file history", path);
    h.add_files(path);
  }

  auto stream = write_file(get_file_path(), FileType::Binary);
  if (!h.SerializeToOstream(&stream)) {
    spdlog::error("Failed to save file history!");
  }
}

void clear_file_history()
{
  spdlog::debug("Clearing file history...");
  history_entries.clear();
}

void add_file_to_history(const Path& path)
{
  auto converted = convert_to_forward_slashes(path);
  if (std::find(history_entries.begin(), history_entries.end(), converted) ==
      history_entries.end()) {
    spdlog::debug("Adding '{}' to history...", converted);
    history_entries.push_back(std::move(converted));

    if (history_entries.size() > history_max_size) {
      history_entries.pop_front();
    }
  }
  else {
    spdlog::debug("Did not add existing entry '{}' to file history", converted);
  }
}

void set_last_closed_file(const Path& path)
{
  history_last_closed_file = convert_to_forward_slashes(path);
  spdlog::debug("Last closed file is now '{}'", *history_last_closed_file);

  add_file_to_history(path);
}

auto file_history() -> const Deque<String>&
{
  return history_entries;
}

auto is_last_closed_file_valid() -> bool
{
  return history_last_closed_file && fs::exists(to_path(*history_last_closed_file));
}

auto last_closed_file() -> const String&
{
  if (is_last_closed_file_valid()) {
    return history_last_closed_file.value();
  }
  else {
    throw TactileError {"Invalid last closed file!"};
  }
}

}  // namespace tactile::io
