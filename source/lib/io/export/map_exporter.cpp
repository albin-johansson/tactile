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

#include "map_exporter.hpp"

#include <exception>  // exception

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/profile.hpp"
#include "io/export/tactile_yaml_exporter.hpp"
#include "io/export/tiled_json_exporter.hpp"
#include "io/export/tiled_xml_exporter.hpp"
#include "io/ir/ir_generation.hpp"
#include "io/save_formats.hpp"
#include "model/documents/document_components.hpp"
#include "model/locator.hpp"
#include "model/persistence/settings_system.hpp"

namespace tactile {

auto save_map_document_to_disk(const Registry& registry, const Entity map_document_entity)
    -> Result
{
  try {
    TACTILE_DEBUG_PROFILE_START

    const auto& document = registry.get<Document>(map_document_entity);
    const auto& settings = Locator<SettingsSystem>::get().current_settings();

    if (!document.path.has_value()) {
      spdlog::error("[IO] Tried to save map with no associated file path");
      return failure;
    }

    const auto path = fs::absolute(*document.path);
    spdlog::info("[IO] Trying to save map to {}", path);

    if (has_supported_tactile_yaml_extension(path)) {
      save_map_as_tactile_yaml(path,
                               convert_map_document_to_ir(registry, map_document_entity),
                               settings);
    }
    else if (has_supported_tiled_json_extension(path)) {
      save_map_as_tiled_json(path,
                             convert_map_document_to_ir(registry, map_document_entity),
                             settings);
    }
    else if (has_supported_tiled_xml_extension(path)) {
      save_map_as_tiled_xml(path,
                            convert_map_document_to_ir(registry, map_document_entity),
                            settings);
    }
    else {
      spdlog::error("[IO] Unsupported file extension {}", path.extension());
      return failure;
    }

    TACTILE_DEBUG_PROFILE_END("Saved map")
    return success;
  }
  catch (const std::exception& e) {
    spdlog::error("[IO] Could not save map to disk: {}", e.what());
    return failure;
  }
  catch (...) {
    spdlog::error("[IO] Could not save map to disk");
    return failure;
  }
}

}  // namespace tactile
