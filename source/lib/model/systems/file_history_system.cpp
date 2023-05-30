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

#include "file_history_system.hpp"

#include <algorithm>  // find
#include <utility>    // move

#include <spdlog/spdlog.h>

#include "common/util/filesystem.hpp"
#include "components/document.hpp"

namespace tactile::sys {

void store_open_documents_in_file_history(Model& model)
{
  auto& file_history = model.get<FileHistory>();

  for (const auto& [document_entity, document]: model.each<Document>()) {
    if (document.type == DocumentType::Map && document.path.has_value()) {
      auto path_str = use_forward_slashes(*document.path);
      if (std::ranges::find(file_history.entries, path_str) ==
          file_history.entries.end()) {
        file_history.entries.push_back(std::move(path_str));
      }
    }
  }
}

void add_to_file_history(FileHistory& history, const Path& path)
{
  auto path_str = use_forward_slashes(path);

  if (std::ranges::find(history.entries, path_str) == history.entries.end()) {
    spdlog::debug("[History] Storing '{}'", path_str);

    history.entries.push_back(std::move(path_str));

    if (history.entries.size() > kFileHistoryMaxSize) {
      history.entries.pop_front();
    }
  }
  else {
    spdlog::debug("[History] Did not add existing entry '{}'", path_str);
  }
}

auto is_last_closed_file_valid(const FileHistory& history) -> bool
{
  return history.last_closed_file.has_value() &&
         fs::exists(Path {*history.last_closed_file});
}

}  // namespace tactile::sys
