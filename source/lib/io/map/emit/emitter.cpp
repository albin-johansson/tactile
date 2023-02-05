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

#include "emitter.hpp"

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/assert.hpp"
#include "common/debug/profile.hpp"
#include "common/util/fmt.hpp"
#include "io/ir/ir.hpp"
#include "io/ir/map_to_ir.hpp"
#include "io/map/emit/gd/godot_converter.hpp"
#include "io/map/emit/gd/godot_options.hpp"
#include "io/map/emit/gd/godot_writer.hpp"
#include "model/document/map_document.hpp"

namespace tactile {

void emit_map(const MapDocument& document)
{
  TACTILE_DEBUG_PROFILE_START
  TACTILE_ASSERT(document.has_path());

  const auto path = fs::absolute(document.get_path());
  const auto ext = path.extension();
  spdlog::info("Trying to save map to {}", path);

  if (ext == ".yaml" || ext == ".yml") {
    emit_yaml_map(path, convert_map_document_to_ir(document));
  }
  else if (ext == ".json" || ext == ".tmj") {
    emit_json_map(path, convert_map_document_to_ir(document));
  }
  else if (ext == ".xml" || ext == ".tmx") {
    emit_xml_map(path, convert_map_document_to_ir(document));
  }
  else {
    spdlog::error("Unsupported file extension {}", ext);
    return;
  }

  TACTILE_DEBUG_PROFILE_END("Saved map")
}

void emit_map_as_godot_scene(const MapDocument& document, const GodotEmitOptions& options)
{
  TACTILE_DEBUG_PROFILE_START

  // TODO validate directories

  const auto root = fs::absolute(options.root_dir);
  const auto path = root / options.project_map_dir / "map.tscn";

  spdlog::debug("Trying to save map as Godot scene at {}", path);

  const auto ir_map = convert_map_document_to_ir(document);
  const auto scene = convert_to_godot(ir_map, options);
  write_godot_scene(scene, options);

  TACTILE_DEBUG_PROFILE_END("Saved map as Godot scene")
}

}  // namespace tactile
