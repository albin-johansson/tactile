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

#pragma once

#include "common/macros.hpp"
#include "common/type/path.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Model)
TACTILE_FWD_DECLARE_STRUCT(MapIR)

/// Restores a map document from an intermediate map representation.
///
/// \param ir_map the intermediate representation of the map data.
/// \param document_path the path to the document.
/// \param model the target document model.
void create_map_document_from_ir(const MapIR& ir_map,
                                 const Path& document_path,
                                 Model& model);

}  // namespace tactile
