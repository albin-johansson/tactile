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

#include "session.hpp"

#include <spdlog/spdlog.h>

#include "common/util/filesystem.hpp"
#include "io/directories.hpp"
#include "io/ir/ir_restoration.hpp"
#include "io/map/parse/parse_map.hpp"
#include "io/proto/proto.hpp"
#include "io/stream.hpp"
#include "model/documents/document_components.hpp"
#include "model/model.hpp"

namespace tactile {
namespace {

constexpr int kSessionFormatVersion [[maybe_unused]] = 1;

[[nodiscard]] auto _get_file_path() -> const Path&
{
  static const auto path = get_persistent_file_dir() / "session.bin";
  return path;
}

}  // namespace

void load_session_from_disk(Model& model)
{
  proto::Session session;

  auto stream = open_input_stream(_get_file_path(), FileType::Binary);
  if (!stream) {
    spdlog::error("[Session] Could not open session file");
    return;
  }

  if (session.ParseFromIstream(&stream.value())) {
    for (const auto& file_path: session.files()) {
      const auto ir_map = parse_map(file_path);
      if (ir_map.has_value()) {
        create_map_document_from_ir(*ir_map, file_path, model);
      }
      else {
        spdlog::warn("[Session] Could not restore map from previous session");
      }
    }
  }
  else {
    spdlog::warn("[Session] Could not parse session file");
  }
}

void save_session_to_disk(const Model& model)
{
  proto::Session session;

  for (auto [document_entity, document]: model.each<Document>()) {
    if (document.type == DocumentType::Map && document.path.has_value()) {
      const auto document_path = fs::absolute(*document.path);
      session.add_files(to_forward_slashes_path(document_path));
    }
  }

  auto stream = open_output_stream(_get_file_path(), FileType::Binary);
  if (stream.has_value()) {
    if (!session.SerializeToOstream(&stream.value())) {
      spdlog::error("[Session] Failed to save session file");
    }
  }
  else {
    spdlog::error("[Session] Failed to open session file for writing");
  }
}

}  // namespace tactile
