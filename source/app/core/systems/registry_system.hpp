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

#include <entt/entity/registry.hpp>

namespace tactile::sys {

/**
 * \brief Creates a registry for a map document.
 *
 * \ingroup systems
 *
 * \details The registry will feature the following context components:
 * - `comp::MapInfo`
 * - `Viewport`
 * - `TilesetContext`
 * - `AttributeContext`
 * - `ActiveLayer`
 * - `ActiveTileset`
 * - `ActiveAttributeContext`
 * - `ActiveObject`
 * - `ToolManager`
 *
 * \return a map document registry.
 */
[[nodiscard]] auto new_map_document_registry() -> entt::registry;

}  // namespace tactile::sys
