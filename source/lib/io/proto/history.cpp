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

#include "common/debug/panic.hpp"
#include "common/numeric.hpp"
#include "common/type/maybe.hpp"
#include "common/util/filesystem.hpp"
#include "common/util/fmt.hpp"
#include "io/directories.hpp"
#include "io/proto/proto.hpp"
#include "io/stream.hpp"

namespace tactile {
namespace {

constexpr int kHistoryFormatVersion [[maybe_unused]] = 1;
constexpr usize kHistoryMaxSize = 10;

// We store paths as strings because that makes displaying them
// in menus much easier (and faster)
struct HistoryState final {
  Maybe<String> last_closed_file;
  Deque<String> entries;
};

inline HistoryState gHistory;

[[nodiscard]] auto get_file_path() -> const Path&
{
  static const auto path = get_persistent_file_dir() / "history.bin";
  return path;
}

}  // namespace

void load_file_history_from_disk()
{
  spdlog::debug("Loading file history...");

  auto stream = open_input_stream(get_file_path(), FileType::Binary);
  if (!stream) {
    spdlog::error("Could not open file history file");
    return;
  }

  proto::History h;
  if (h.ParseFromIstream(&stream.value())) {
    if (h.has_last_opened_file()) {
      gHistory.last_closed_file = h.last_opened_file();
    }

    for (const auto& file: h.files()) {
      if (fs::exists(file)) {
        spdlog::debug("Loaded '{}' from file history", file);
        gHistory.entries.push_back(file);
      }
    }
  }
  else {
    spdlog::warn("Failed to read history file (this is expected for first time runs)");
  }
}

void save_file_history_to_disk()
{
  proto::History h;

  if (gHistory.last_closed_file) {
    h.set_last_opened_file(*gHistory.last_closed_file);
  }

  for (const auto& path: gHistory.entries) {
    spdlog::debug("Saving '{}' to file history", path);
    h.add_files(path);
  }

  auto stream = open_output_stream(get_file_path(), FileType::Binary);
  if (!stream) {
    spdlog::error("Could not open file history file for writing");
    return;
  }

  if (!h.SerializeToOstream(&stream.value())) {
    spdlog::error("Failed to save file history");
  }
}

void clear_file_history()
{
  spdlog::debug("Clearing file history...");
  gHistory.entries.clear();
}

void add_to_file_history(const Path& path)
{
  auto converted = use_forward_slashes(path);
  if (std::find(gHistory.entries.begin(), gHistory.entries.end(), converted) ==
      gHistory.entries.end()) {
    spdlog::debug("Adding '{}' to history...", converted);
    gHistory.entries.push_back(std::move(converted));

    if (gHistory.entries.size() > kHistoryMaxSize) {
      gHistory.entries.pop_front();
    }
  }
  else {
    spdlog::debug("Did not add existing entry '{}' to file history", converted);
  }
}

void set_last_closed_file(const Path& path)
{
  gHistory.last_closed_file = use_forward_slashes(path);
  spdlog::debug("Last closed file is now '{}'", *gHistory.last_closed_file);

  add_to_file_history(path);
}

auto get_file_history() -> const Deque<String>&
{
  return gHistory.entries;
}

auto get_last_closed_file() -> const String&
{
  if (is_last_closed_file_valid()) {
    return gHistory.last_closed_file.value();
  }
  else {
    throw TactileError {"Invalid last closed file!"};
  }
}

auto is_last_closed_file_valid() -> bool
{
  return gHistory.last_closed_file && fs::exists(Path {*gHistory.last_closed_file});
}

}  // namespace tactile
