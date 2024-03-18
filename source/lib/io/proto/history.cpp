// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "history.hpp"

#include <spdlog/spdlog.h>

#include "io/proto/proto.hpp"
#include "io/stream.hpp"
#include "model/file_history.hpp"
#include "tactile/core/platform/filesystem.hpp"

namespace tactile {
namespace {

constexpr int kHistoryFormatVersion [[maybe_unused]] = 1;

[[nodiscard]] auto get_file_path() -> const Path&
{
  static const auto path = get_persistent_storage_directory().value() / "history.bin";
  return path;
}

}  // namespace

auto load_file_history_from_disk() -> Maybe<FileHistory>
{
  spdlog::debug("Loading file history...");

  auto stream = open_input_stream(get_file_path(), FileType::Binary);
  if (!stream) {
    spdlog::error("Could not open file history file");
    return nothing;
  }

  FileHistory file_history;
  proto::History h;

  if (h.ParseFromIstream(&stream.value())) {
    if (h.has_last_opened_file()) {
      file_history.last_closed_file = h.last_opened_file();
    }

    for (const auto& file : h.files()) {
      if (fs::exists(file)) {
        spdlog::debug("Loaded '{}' from file history", file);
        file_history.entries.push_back(file);
      }
    }
  }
  else {
    spdlog::warn("Failed to read history file (this is expected for first time runs)");
  }

  return file_history;
}

void save_file_history_to_disk(const FileHistory& history)
{
  proto::History h;

  if (history.last_closed_file) {
    h.set_last_opened_file(*history.last_closed_file);
  }

  for (const auto& path : history.entries) {
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

}  // namespace tactile
