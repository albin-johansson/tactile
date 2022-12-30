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

#include <spdlog/spdlog.h>

#include "core/util/filesystem.hpp"
#include "io/directories.hpp"
#include "io/file.hpp"
#include "io/map/ir/map_from_ir.hpp"
#include "io/map/parse/parse_map.hpp"
#include "io/proto/proto.hpp"
#include "model/document/map_document.hpp"
#include "model/model.hpp"

namespace tactile::io {
namespace {

constexpr int session_format_version [[maybe_unused]] = 1;

[[nodiscard]] auto get_file_path() -> const Path&
{
  static const auto path = persistent_file_dir() / "session.bin";
  return path;
}

}  // namespace

void session_restore_previous(DocumentModel& model)
{
  proto::Session session;

  auto stream = read_file(get_file_path(), FileType::Binary);
  if (session.ParseFromIstream(&stream)) {
    for (const auto& file: session.files()) {
      const auto ir = parse_map(file);
      if (ir.error() == ParseError::None) {
        map_from_ir(ir, model);
      }
      else {
        spdlog::warn("Failed to restore map from last session!");
      }
    }
  }
  else {
    spdlog::warn("Failed to parse binary session file!");
  }
}

void session_save(const DocumentModel& model)
{
  proto::Session session;
  model.each([&](const UUID& document_id) {
    if (model.is_map(document_id)) {
      const auto& map_doc = model.get_map_document(document_id);
      if (map_doc.has_path()) {
        const auto document_path = fs::absolute(map_doc.get_path());
        session.add_files(convert_to_forward_slashes(document_path));
      }
    }
  });

  auto stream = write_file(get_file_path(), FileType::Binary);
  if (!session.SerializeToOstream(&stream)) {
    spdlog::error("Failed to save session file!");
  }
}

}  // namespace tactile::io
