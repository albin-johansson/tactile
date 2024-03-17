// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "file_history.hpp"

#include <spdlog/spdlog.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/platform/filesystem.hpp"

namespace tactile {
namespace {

constexpr usize kHistoryMaxSize = 10;

inline FileHistory gHistory;

}  // namespace

void clear_file_history()
{
  spdlog::debug("Clearing file history...");
  gHistory.entries.clear();
}

void add_to_file_history(const Path& path)
{
  auto converted = normalize_path(path);
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
  gHistory.last_closed_file = normalize_path(path);
  spdlog::debug("Last closed file is now '{}'", *gHistory.last_closed_file);

  add_to_file_history(path);
}

auto get_last_closed_file() -> const String&
{
  if (is_last_closed_file_valid()) {
    return gHistory.last_closed_file.value();
  }
  else {
    throw Exception {"Invalid last closed file!"};
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
