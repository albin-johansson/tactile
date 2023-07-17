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

#include "entity_validation.hpp"

#include "cmd/command_stack.hpp"
#include "model/components/component_components.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/objects/object_components.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile::sys {

auto is_document_entity(const Registry& registry, const Entity entity) -> bool
{
  return is_map_document_entity(registry, entity) ||
         is_tileset_document_entity(registry, entity);
}

auto is_map_document_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&                      //
         registry.has<MapDocument>(entity) &&          //
         registry.has<Document>(entity) &&             //
         registry.has<Viewport>(entity) &&             //
         registry.has<DynamicViewportInfo>(entity) &&  //
         registry.has<CommandStack>(entity);
}

auto is_tileset_document_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&                      //
         registry.has<TilesetDocument>(entity) &&      //
         registry.has<Document>(entity) &&             //
         registry.has<Viewport>(entity) &&             //
         registry.has<DynamicViewportInfo>(entity) &&  //
         registry.has<CommandStack>(entity);
}

auto is_context_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity && registry.has<Context>(entity);
}

auto is_map_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&                   //
         registry.has<Context>(entity) &&           //
         registry.has<Map>(entity) &&               //
         registry.has<MapIdentifiers>(entity) &&    //
         registry.has<MapLayerSuffixes>(entity) &&  //
         registry.has<TileFormat>(entity);
}

auto is_component_set_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity && registry.has<ComponentSet>(entity);
}

auto is_component_entity(const Registry& registry, Entity entity) -> bool
{
  return entity != kNullEntity && registry.has<Component>(entity);
}

auto is_attached_component_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity && registry.has<AttachedComponent>(entity);
}

auto is_layer_entity(const Registry& registry, const Entity entity) -> bool
{
  return is_tile_layer_entity(registry, entity) ||    //
         is_object_layer_entity(registry, entity) ||  //
         is_group_layer_entity(registry, entity);
}

auto is_tile_layer_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&            //
         registry.has<TileLayer>(entity) &&  //
         registry.has<Layer>(entity) &&      //
         registry.has<Context>(entity);
}

auto is_object_layer_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&              //
         registry.has<ObjectLayer>(entity) &&  //
         registry.has<Layer>(entity) &&        //
         registry.has<Context>(entity);
}

auto is_group_layer_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&             //
         registry.has<GroupLayer>(entity) &&  //
         registry.has<Layer>(entity) &&       //
         registry.has<Context>(entity);
}

auto is_object_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&         //
         registry.has<Object>(entity) &&  //
         registry.has<Context>(entity);
}

auto is_texture_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity && registry.has<Texture>(entity) &&
         (registry.has<NullTexture>(entity) ||    //
          registry.has<OpenGLTexture>(entity) ||  //
          registry.has<VulkanTexture>(entity));
}

auto is_tileset_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&                     //
         registry.has<Tileset>(entity) &&             //
         registry.has<TilesetRenderCache>(entity) &&  //
         registry.has<Context>(entity);
}

auto is_attached_tileset_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&                  //
         registry.has<AttachedTileset>(entity) &&  //
         registry.has<Viewport>(entity) &&         //
         registry.has<DynamicViewportInfo>(entity);
}

auto is_tile_entity(const Registry& registry, const Entity entity) -> bool
{
  return entity != kNullEntity &&          //
         registry.has<Context>(entity) &&  //
         registry.has<Tile>(entity);
}

}  // namespace tactile::sys
