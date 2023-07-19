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

#include "document_factory.hpp"

#include "cmd/command_stack.hpp"
#include "common/debug/assert.hpp"
#include "model/documents/document_components.hpp"
#include "model/entity_validation.hpp"
#include "model/maps/map_factory.hpp"
#include "model/persistence/settings_system.hpp"
#include "model/services/locator.hpp"
#include "model/tilesets/tileset_factory.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile::sys {

auto create_map_document(Registry& registry,
                         const TileExtent& extent,
                         const Int2& tile_size) -> Entity
{
  const auto& settings_system = Locator<SettingsSystem>::get();
  const auto& settings = settings_system.current_settings();

  const auto document_entity = registry.create_entity();

  auto& command_stack = registry.add<CommandStack>(document_entity);
  command_stack.set_capacity(settings.get_command_capacity());

  auto& map_document = registry.add<MapDocument>(document_entity);
  map_document.map = create_map(registry, extent, tile_size);
  map_document.active_tileset = kNullEntity;

  auto& document = registry.add<Document>(document_entity);
  document.type = DocumentType::Map;
  document.default_context = map_document.map;
  document.active_context = document.default_context;

  auto& document_viewport = registry.add<Viewport>(document_entity);
  document_viewport.offset = Float2 {0, 0};
  document_viewport.tile_size = tile_size;

  registry.add<DynamicViewportInfo>(document_entity);

  TACTILE_ASSERT(is_map_document_entity(registry, document_entity));
  return document_entity;
}

auto create_tileset_document(Registry& registry,
                             const Int2& tile_size,
                             const Path& image_path) -> Entity
{
  const auto& settings_system = Locator<SettingsSystem>::get();
  const auto& settings = settings_system.current_settings();

  const auto document_entity = registry.create_entity();

  auto& command_stack = registry.add<CommandStack>(document_entity);
  command_stack.set_capacity(settings.get_command_capacity());

  auto& tileset_document = registry.add<TilesetDocument>(document_entity);
  tileset_document.tileset = create_tileset(registry, tile_size, image_path);

  auto& document = registry.add<Document>(document_entity);
  document.type = DocumentType::Tileset;
  document.default_context = tileset_document.tileset;
  document.active_context = document.default_context;

  auto& document_viewport = registry.add<Viewport>(document_entity);
  document_viewport.offset = Float2 {0, 0};
  document_viewport.tile_size = tile_size;

  registry.add<DynamicViewportInfo>(document_entity);

  TACTILE_ASSERT(is_tileset_document_entity(registry, document_entity));
  return document_entity;
}

}  // namespace tactile::sys
