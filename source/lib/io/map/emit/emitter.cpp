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

#include "common/debug/profile.hpp"
#include "common/util/fmt.hpp"
#include "io/export/json_exporter.hpp"
#include "io/export/xml_exporter.hpp"
#include "io/export/yaml_exporter.hpp"
#include "io/ir/ir.hpp"
#include "io/ir/map_to_ir.hpp"
#include "io/map/emit/gd/godot_converter.hpp"
#include "io/map/emit/gd/godot_options.hpp"
#include "io/map/emit/gd/godot_writer.hpp"
#include "io/util/json.hpp"
#include "io/util/xml.hpp"
#include "io/util/yaml.hpp"
#include "model/document/map_document.hpp"

namespace tactile {

void save_map_document_to_disk(const MapDocument& document)
{
  TACTILE_DEBUG_PROFILE_START

  if (!document.has_path()) {
    spdlog::error("Tried to save map document with no associated file path");
    return;
  }

  const auto path = fs::absolute(document.get_path());
  spdlog::info("Trying to save map to {}", path);

  if (has_supported_tactile_yaml_extension(path)) {
    save_map_as_tactile_yaml(path, convert_map_document_to_ir(document));
  }
  else if (has_supported_tiled_json_extension(path)) {
    save_map_as_tiled_json(path, convert_map_document_to_ir(document));
  }
  else if (has_supported_tiled_xml_extension(path)) {
    save_map_as_tiled_xml(path, convert_map_document_to_ir(document));
  }
  else {
    spdlog::error("Unsupported file extension {}", path.extension());
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
