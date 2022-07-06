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

#include "map_document.hpp"

#include <algorithm>  // find_if
#include <utility>    // move

#include "core/commands/layers/all.hpp"
#include "core/commands/maps/all.hpp"
#include "core/commands/objects/all.hpp"
#include "core/commands/tools/all.hpp"
#include "misc/panic.hpp"

namespace tactile {

MapDocument::MapDocument(const Vector2i& tileSize, const usize rows, const usize columns)
    : mMap{std::make_shared<Map>()}
{
  mMap->resize(rows, columns);
  mMap->set_tile_size(tileSize);

  register_context(mMap);
  select_context(mMap->get_uuid());
}

void MapDocument::register_context(Shared<IContext> context)
{
  const auto id = context->get_uuid();
  mContexts[id] = std::move(context);
}

void MapDocument::unregister_context(const UUID& id)
{
  if (const auto iter = mContexts.find(id); iter != mContexts.end()) {
    mContexts.erase(iter);

    if (mActiveContext == id) {
      mActiveContext = mMap->get_uuid();
    }
  }
  else {
    throw TactileError{"Tried to remove non-existent context!"};
  }
}

auto MapDocument::get_context(const UUID& id) -> Shared<IContext>
{
  return lookup_in(mContexts, id);
}

auto MapDocument::view_context(const UUID& id) const -> const IContext&
{
  return *lookup_in(mContexts, id);
}

void MapDocument::update()
{
  for (auto& [id, ref] : mMap->get_tilesets()) {
    ref.update();
  }
}

void MapDocument::add_row()
{
  get_history().exec<AddRowCmd>(this);
}

void MapDocument::add_column()
{
  get_history().exec<AddColumnCmd>(this);
}

void MapDocument::remove_row()
{
  get_history().exec<RemoveRowCmd>(this);
}

void MapDocument::remove_column()
{
  get_history().exec<RemoveColumnCmd>(mMap);
}

void MapDocument::resize(const usize rows, const usize cols)
{
  get_history().exec<ResizeMapCmd>(this, rows, cols);
}

void MapDocument::fix_tiles()
{
  get_history().exec<FixTilesInMapCmd>(this);
}

void MapDocument::add_layer(const LayerType type)
{
  get_history().exec<AddLayerCmd>(this, type);
}

void MapDocument::remove_layer(const UUID& layerId)
{
  get_history().exec<RemoveLayerCmd>(this, layerId);
}

void MapDocument::duplicate_layer(const UUID& layerId)
{
  get_history().exec<DuplicateLayerCmd>(this, layerId);
}

void MapDocument::rename_layer(const UUID& layerId, std::string name)
{
  get_history().exec<RenameLayerCmd>(this, layerId, std::move(name));
}

void MapDocument::move_layer_up(const UUID& layerId)
{
  get_history().exec<MoveLayerUpCmd>(this, layerId);
}

void MapDocument::move_layer_down(const UUID& layerId)
{
  get_history().exec<MoveLayerDownCmd>(this, layerId);
}

void MapDocument::set_layer_opacity(const UUID& layerId, const float opacity)
{
  get_history().exec<SetLayerOpacityCmd>(this, layerId, opacity);
}

void MapDocument::set_layer_visible(const UUID& layerId, const bool visible)
{
  get_history().exec<SetLayerVisibilityCmd>(this, layerId, visible);
}

void MapDocument::register_stamp_sequence(const UUID& layerId,
                                          TileCache   previous,
                                          TileCache   sequence)
{
  get_history().store<StampToolCmd>(this,
                                    layerId,
                                    std::move(previous),
                                    std::move(sequence));
}

void MapDocument::register_eraser_sequence(const UUID& layerId, TileCache previous)
{
  get_history().store<EraserToolCmd>(this, layerId, std::move(previous));
}

void MapDocument::flood(const UUID&    layerId,
                        const TilePos& origin,
                        const TileID   replacement)
{
  get_history().exec<BucketToolCmd>(this, layerId, origin, replacement);
}

void MapDocument::add_rectangle(const UUID&     layerId,
                                const Vector2f& pos,
                                const Vector2f& size)
{
  get_history().exec<AddObjectCmd>(this, layerId, ObjectType::Rect, pos, size);
}

void MapDocument::add_ellipse(const UUID&     layerId,
                              const Vector2f& pos,
                              const Vector2f& size)
{
  get_history().exec<AddObjectCmd>(this, layerId, ObjectType::Ellipse, pos, size);
}

void MapDocument::add_point(const UUID& layerId, const Vector2f& pos)
{
  get_history().exec<AddObjectCmd>(this, layerId, ObjectType::Point, pos);
}

void MapDocument::move_object(const UUID&     objectId,
                              const Vector2f& previous,
                              const Vector2f& updated)
{
  get_history().exec<MoveObjectCmd>(this, objectId, previous, updated);
}

void MapDocument::set_object_visible(const UUID& objectId, const bool visible)
{
  get_history().exec<SetObjectVisibleCmd>(this, objectId, visible);
}

void MapDocument::set_object_name(const UUID& objectId, std::string name)
{
  get_history().exec<SetObjectNameCmd>(this, objectId, std::move(name));
}

void MapDocument::set_object_tag(const UUID& objectId, std::string tag)
{
  get_history().exec<SetObjectTagCmd>(this, objectId, std::move(tag));
}

auto MapDocument::get_object(const UUID& objectId) -> Shared<Object>
{
  if (auto ptr = std::dynamic_pointer_cast<Object>(get_context(objectId))) {
    return ptr;
  }
  else {
    throw TactileError{"Did not find object!"};
  }
}

void MapDocument::set_name(std::string name)
{
  mMap->set_name(name);
}

auto MapDocument::get_name() const -> const std::string&
{
  return mMap->get_name();
}

auto MapDocument::get_tools() -> ToolManager&
{
  return mTools;
}

auto MapDocument::get_tools() const -> const ToolManager&
{
  return mTools;
}

}  // namespace tactile