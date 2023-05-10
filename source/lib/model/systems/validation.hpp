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

#include "common/type/ecs.hpp"
#include "model/model.hpp"

namespace tactile::sys {

[[nodiscard]] auto is_document_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_map_document_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_tileset_document_entity(const Model& model, Entity entity) -> bool;

/// Indicates whether an entity is a valid context entity.
[[nodiscard]] auto is_context_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_map_entity(const Model& model, Entity entity) -> bool;

/// Indicates whether an entity is a valid component set entity.
[[nodiscard]] auto is_component_set_entity(const Model& model, Entity entity) -> bool;

/// Indicates whether an entity is a valid component definition entity.
[[nodiscard]] auto is_component_definition_entity(const Model& model, Entity entity)
    -> bool;

/// Indicates whether an entity is a valid component entity.
[[nodiscard]] auto is_component_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_layer_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_tile_layer_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_object_layer_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_group_layer_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_object_entity(const Model& model, Entity entity) -> bool;

/// Indicates whether an entity is a valid texture entity.
[[nodiscard]] auto is_texture_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_tileset_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_attached_tileset_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto is_tile_entity(const Model& model, Entity entity) -> bool;

}  // namespace tactile::sys
