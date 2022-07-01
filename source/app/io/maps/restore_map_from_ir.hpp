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

#pragma once

#include "core/fwd.hpp"
#include "editor/fwd.hpp"
#include "io/fwd.hpp"

namespace tactile::io {

/**
 * Restores a map document from an intermediate map representation.
 *
 * \param data the intermediate representation of the map data.
 * \param model the target document model.
 * \param textures the texture manager that will be used.
 */
void restore_map_from_ir(const ParseData& data,
                         DocumentModel&   model,
                         TextureManager&  textures);

}  // namespace tactile::io
