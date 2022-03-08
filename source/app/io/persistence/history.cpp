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

#include <algorithm>   // find, find_if
#include <filesystem>  // path, exists
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios
#include <utility>     // move

#include "core/utils/strings.hpp"
#include "io/directories.hpp"
#include "misc/logging.hpp"
#include "misc/throw.hpp"
#include "proto.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

// constexpr int _format_version = 1;
constexpr usize _max_size = 10;

/* We store paths as strings because that makes displaying them in menus
   _much_ easier (and faster) */
inline maybe<std::string> _last_closed_file;
inline std::deque<std::string> _history;

[[nodiscard]] auto get_file_path() -> const std::filesystem::path&
{
  static const auto path = persistent_file_dir() / "history.bin";
  return path;
}

}  // namespace

void load_file_history()
{
  log_verbose("Loading file history...");
  std::ifstream stream{get_file_path(), std::ios::in | std::ios::binary};

  proto::history h;
  if (h.ParseFromIstream(&stream)) {
    if (h.has_last_opened_file()) {
      _last_closed_file = h.last_opened_file();
    }

    for (auto file : h.files()) {
      log_debug("Loaded '{}' from file history", file);
      _history.push_back(std::move(file));
    }
  }
  else {
    log_warning("Failed to read history file (this is expected for first time runs)");
  }
}

void save_file_history()
{
  proto::history h;

  if (_last_closed_file) {
    h.set_last_opened_file(*_last_closed_file);
  }

  for (const auto& path : _history) {
    log_debug("Saving '{}' to file history", path);
    h.add_files(path);
  }

  {
    std::ofstream stream{get_file_path(),
                         std::ios::out | std::ios::trunc | std::ios::binary};
    if (!h.SerializeToOstream(&stream)) {
      log_error("Failed to save file history!");
    }
  }
}

void clear_file_history()
{
  log_verbose("Clearing file history...");
  _history.clear();
}

void add_file_to_history(const std::filesystem::path& path)
{
  auto converted = convert_to_forward_slashes(path);
  if (std::find(_history.begin(), _history.end(), converted) == _history.end()) {
    log_debug("Adding '{}' to history...", converted);
    _history.push_back(std::move(converted));

    if (_history.size() > _max_size) {
      _history.pop_front();
    }
  }
  else {
    log_debug("Did not add existing entry '{}' to file history", converted);
  }
}

void set_last_closed_file(const std::filesystem::path& path)
{
  _last_closed_file = convert_to_forward_slashes(path);
  log_verbose("Last closed file is now '{}'", *_last_closed_file);

  add_file_to_history(path);
}

auto file_history() -> const std::deque<std::string>&
{
  return _history;
}

auto is_last_closed_file_valid() -> bool
{
  return _last_closed_file && std::filesystem::exists(*_last_closed_file);
}

auto last_closed_file() -> const std::string&
{
  if (is_last_closed_file_valid()) {
    return _last_closed_file.value();
  }
  else {
    throw_traced(tactile_error{"Invalid last closed file!"});
  }
}

}  // namespace tactile
