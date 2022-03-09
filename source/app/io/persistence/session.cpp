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

#include "session.hpp"

#include <filesystem>  // path, absolute
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios

#include "core/utils/strings.hpp"
#include "editor/model.hpp"
#include "io/directories.hpp"
#include "io/maps/parser/parse_map.hpp"
#include "io/maps/restore_document_from_ir.hpp"
#include "misc/logging.hpp"
#include "proto.hpp"

namespace tactile {
namespace {

constexpr int _format_version [[maybe_unused]] = 1;

[[nodiscard]] auto _get_file_path() -> const std::filesystem::path&
{
  static const auto path = persistent_file_dir() / "session.bin";
  return path;
}

}  // namespace

void restore_last_session(DocumentModel& model, texture_manager& textures)
{
  proto::session session;

  std::ifstream stream{_get_file_path(), std::ios::in | std::ios::binary};
  if (session.ParseFromIstream(&stream)) {
    for (const auto& file : session.files()) {
      const auto ir = parsing::parse_map(file);
      if (ir.error() == parsing::parse_error::none) {
        model.add_map(restore_document_from_ir(ir, textures));
      }
      else {
        log_warning("Failed to restore map from last session!");
      }
    }
  }
  else {
    log_warning("Failed to parse binary session file!");
  }
}

void save_session(const DocumentModel& model)
{
  proto::session session;
  for (const auto& [id, document] : model) {
    if (!document->path.empty()) {
      const auto documentPath = std::filesystem::absolute(document->path);
      session.add_files(convert_to_forward_slashes(documentPath));
    }
  }

  std::ofstream stream{_get_file_path(),
                       std::ios::out | std::ios::trunc | std::ios::binary};
  if (!session.SerializeToOstream(&stream)) {
    log_error("Failed to save session file!");
  }
}

}  // namespace tactile
