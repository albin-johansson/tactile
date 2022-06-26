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

#include "save_document.hpp"

#include <filesystem>  // absolute

#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

#include "core/documents/map_document.hpp"
#include "io/maps/convert_map_to_ir.hpp"
#include "io/maps/emitter/emit_info.hpp"
#include "io/maps/emitter/json_emitter.hpp"
#include "io/maps/emitter/xml_emitter.hpp"
#include "io/maps/emitter/yaml_emitter.hpp"
#include "meta/profile.hpp"
#include "misc/assert.hpp"

namespace tactile::io {

void save_document(const MapDocument& document)
{
  TACTILE_PROFILE_START
  TACTILE_ASSERT(document.has_path());

  const auto path = std::filesystem::absolute(document.get_path());
  spdlog::info("Trying to save map to {}", path);

  EmitInfo info{path, convert_map_to_ir(document)};

  const auto ext = path.extension();
  if (ext == ".yaml" || ext == ".yml") {
    emit_yaml_map(info);
  }
  else if (ext == ".json" || ext == ".tmj") {
    emit_json_map(info);
  }
  else if (ext == ".xml" || ext == ".tmx") {
    emit_xml_map(info);
  }
  else {
    spdlog::error("Unsupported file extension {}", ext);
  }

  TACTILE_PROFILE_END("Emitted document")
}

}  // namespace tactile::io
