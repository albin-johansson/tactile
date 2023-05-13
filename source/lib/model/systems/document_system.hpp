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

#include <utility>  // forward

#include "cmd/command_stack.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "core/tile_extent.hpp"
#include "model/model.hpp"

namespace tactile::sys {

[[nodiscard]] auto create_map_document(Model& model,
                                       const TileExtent& extent,
                                       const Int2& tile_size) -> Entity;

[[nodiscard]] auto create_tileset_document(Model& model,
                                           const Int2& tile_size,
                                           const Path& image_path) -> Entity;

void destroy_document(Model& model, DocumentEntity document_entity);

void select_document(Model& model, Entity document_entity);

void open_document(Model& model, Entity document_entity);

void close_document(Model& model, Entity document_entity);

[[nodiscard]] auto get_document_name(const Model& model, Entity document_entity)
    -> String;

[[nodiscard]] auto get_active_document(const Model& model) -> Entity;

[[nodiscard]] auto get_associated_tileset_document(const Model& model,
                                                   Entity tileset_entity) -> Entity;

[[nodiscard]] auto has_active_document(const Model& model) -> bool;

[[nodiscard]] auto is_map_document_active(const Model& model) -> bool;
[[nodiscard]] auto is_tileset_document_active(const Model& model) -> bool;

[[nodiscard]] auto is_document_open(const Model& model, Entity document_entity) -> bool;

template <typename T, typename... Args>
void try_execute(Model& model, Args&&... args)
{
  const auto document_entity = get_active_document(model);
  if (document_entity != kNullEntity) {
    auto& commands = model.get<CommandStack>(document_entity);
    commands.push<T>(std::forward<Args>(args)...);
  }
}

}  // namespace tactile::sys
