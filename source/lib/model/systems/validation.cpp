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

#include "validation.hpp"

#include "cmd/command_stack.hpp"
#include "components/document.hpp"
#include "core/component.hpp"
#include "core/context.hpp"
#include "core/layer.hpp"
#include "core/map.hpp"
#include "core/object.hpp"
#include "core/texture.hpp"
#include "core/tile.hpp"
#include "core/tile_format.hpp"
#include "core/tileset.hpp"
#include "core/viewport.hpp"

namespace tactile::sys {

auto is_document_entity(const Model& model, const Entity entity) -> bool
{
  return is_map_document_entity(model, entity) ||
         is_tileset_document_entity(model, entity);
}

auto is_map_document_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&           //
         model.has<MapDocument>(entity) &&  //
         model.has<Document>(entity) &&     //
         model.has<Viewport>(entity) &&     //
         model.has<CommandStack>(entity);
}

auto is_tileset_document_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&               //
         model.has<TilesetDocument>(entity) &&  //
         model.has<Document>(entity) &&         //
         model.has<Viewport>(entity) &&         //
         model.has<CommandStack>(entity);
}

auto is_context_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<Context>(entity);
}

auto is_map_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&       //
         model.has<Context>(entity) &&  //
         model.has<Map>(entity) &&      //
         model.has<TileFormat>(entity);
}

auto is_component_set_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<ComponentSet>(entity);
}

auto is_component_definition_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<ComponentDefinition>(entity);
}

auto is_component_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<Component>(entity);
}

auto is_layer_entity(const Model& model, const Entity entity) -> bool
{
  return is_tile_layer_entity(model, entity) ||    //
         is_object_layer_entity(model, entity) ||  //
         is_group_layer_entity(model, entity);
}

auto is_tile_layer_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&         //
         model.has<TileLayer>(entity) &&  //
         model.has<Layer>(entity) &&      //
         model.has<Context>(entity);
}

auto is_object_layer_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&           //
         model.has<ObjectLayer>(entity) &&  //
         model.has<Layer>(entity) &&        //
         model.has<Context>(entity);
}

auto is_group_layer_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&          //
         model.has<GroupLayer>(entity) &&  //
         model.has<Layer>(entity) &&       //
         model.has<Context>(entity);
}

auto is_object_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&      //
         model.has<Object>(entity) &&  //
         model.has<Context>(entity);
}

auto is_texture_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<Texture>(entity) &&
         (model.has<OpenGLTexture>(entity) || model.has<VulkanTexture>(entity));
}

auto is_tileset_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&                  //
         model.has<Tileset>(entity) &&             //
         model.has<TilesetRenderCache>(entity) &&  //
         model.has<Context>(entity);
}

auto is_attached_tileset_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&               //
         model.has<AttachedTileset>(entity) &&  //
         model.has<Viewport>(entity);
}

auto is_tile_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity &&       //
         model.has<Context>(entity) &&  //
         model.has<Tile>(entity);
}

}  // namespace tactile::sys
