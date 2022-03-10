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

#include "emitter/emit_info.hpp"
#include "emitter/json_emitter.hpp"
#include "emitter/xml_emitter.hpp"
#include "emitter/yaml_emitter.hpp"
#include "io/maps/convert_document_to_ir.hpp"
#include "io/persistence/preferences.hpp"
#include "meta/profile.hpp"
#include "misc/assert.hpp"
#include "misc/logging.hpp"

namespace tactile {

void save_document(const Document& document)
{
  TACTILE_ASSERT(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  log_info("Trying to save map to {}", path);

  emitter::EmitInfo info{path, convert_document_to_ir(document)};

  const auto ext = path.extension();
  if (ext == ".yaml" || ext == ".yml") {
    emit_yaml_map(info);
  }
  else if (ext == ".json") {
    emit_json_map(info);
  }
  else if (ext == ".xml" || ext == ".tmx") {
    emit_xml_map(info);
  }
  else {
    log_error("Unsupported file extension {}", ext);
  }

  TACTILE_PROFILE_END("Emitted document")
}

}  // namespace tactile
