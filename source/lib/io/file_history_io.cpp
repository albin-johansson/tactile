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

#include "file_history_io.hpp"

#include <algorithm>  // find

#include <spdlog/spdlog.h>

#include "io/directories.hpp"
#include "io/protobuf_types.hpp"
#include "io/stream_utils.hpp"

namespace tactile {
namespace {

constexpr int kHistoryFormatVersion [[maybe_unused]] = 1;

[[nodiscard]] auto _get_file_path() -> const Path&
{
  static const auto path = get_persistent_file_dir() / "history.bin";
  return path;
}

}  // namespace

auto load_file_history_from_disk() -> Maybe<FileHistory>
{
  spdlog::debug("[History] Loading file history from disk");

  auto stream = open_input_stream(_get_file_path(), FileType::Binary);
  if (!stream) {
    spdlog::error("[History] Could not open file history file");
    return kNone;
  }

  FileHistory file_history;
  proto::History h;

  if (h.ParseFromIstream(&stream.value())) {
    if (h.has_last_opened_file()) {
      file_history.last_closed_file = h.last_opened_file();
    }

    for (const auto& file: h.files()) {
      if (fs::exists(file) &&
          std::ranges::find(file_history.entries, file) == file_history.entries.end()) {
        spdlog::trace("[History] Found '{}'", file);
        file_history.entries.push_back(file);
      }
    }
  }
  else {
    spdlog::warn("[History] Could not read history file (expected for first time runs)");
  }

  return file_history;
}

void save_file_history_to_disk(const FileHistory& history)
{
  proto::History h;

  if (history.last_closed_file) {
    h.set_last_opened_file(*history.last_closed_file);
  }

  for (const auto& path: history.entries) {
    h.add_files(path);
  }

  auto stream = open_output_stream(_get_file_path(), FileType::Binary);
  if (!stream) {
    spdlog::error("[History] Could not write to file history file");
    return;
  }

  if (!h.SerializeToOstream(&stream.value())) {
    spdlog::error("[History] Could not save file history");
  }
}

}  // namespace tactile
