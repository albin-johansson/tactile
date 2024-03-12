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

#include "io/export/tactile_yaml_exporter.hpp"
#include "io/export/tiled_json_exporter.hpp"
#include "io/export/tiled_xml_exporter.hpp"
#include "io/ir/map_document_to_ir.hpp"
#include "model/document/map_document.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/debug/performance.hpp"
#include "tactile/core/io/save/save_format_id.hpp"

namespace tactile {

auto save_map_document_to_disk(const MapDocument& document) -> Result<void>
{
  try {
    TACTILE_DEBUG_PROFILE_SCOPE("save_map_document_to_disk");

    if (!document.has_path()) {
      spdlog::error("Tried to save map document with no associated file path");
      return unexpected(make_error(GenericError::kInvalidState));
    }

    const auto path = fs::absolute(document.get_path());
    spdlog::info("Trying to save map to {}", path);

    if (has_valid_tactile_yaml_extension(path)) {
      save_map_as_tactile_yaml(path, convert_map_document_to_ir(document));
    }
    else if (has_valid_tiled_json_extension(path)) {
      save_map_as_tiled_json(path, convert_map_document_to_ir(document));
    }
    else if (has_valid_tiled_xml_extension(path)) {
      save_map_as_tiled_xml(path, convert_map_document_to_ir(document));
    }
    else {
      spdlog::error("Unsupported file extension {}", path.extension());
      return unexpected(make_error(GenericError::kInvalidParam));
    }

    return kOK;
  }
  catch (const std::exception& e) {
    spdlog::error("Could not save map document to disk: {}", e.what());
    return unexpected(make_error(GenericError::kInvalidFile));
  }
  catch (...) {
    spdlog::error("Could not save map document to disk");
    return unexpected(make_error(GenericError::kInvalidFile));
  }
}

}  // namespace tactile
