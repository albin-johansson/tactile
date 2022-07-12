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

#include "core/cmd/layer/all.hpp"
#include "core/cmd/map/all.hpp"
#include "core/cmd/object/all.hpp"
#include "core/cmd/tool/all.hpp"
#include "misc/panic.hpp"

namespace tactile {

MapDocument::MapDocument(const Vector2i& tileSize, const usize rows, const usize columns)
    : mMap {std::make_shared<Map>()}
    , mContexts {mMap->get_uuid()}
{
  mMap->resize(rows, columns);
  mMap->set_tile_size(tileSize);

  mContexts.add_context(mMap);
  mContexts.select(mMap->get_uuid());
}

auto MapDocument::get_contexts() -> ContextManager&
{
  return mContexts;
}

auto MapDocument::get_contexts() const -> const ContextManager&
{
  return mContexts;
}

void MapDocument::update()
{
  for (auto& [id, ref] : mMap->get_tilesets()) {
    ref.update();
  }
}

void MapDocument::add_row()
{
  get_history().exec<AddRowCmd>(mMap);
}

void MapDocument::add_column()
{
  get_history().exec<AddColumnCmd>(mMap);
}

void MapDocument::remove_row()
{
  get_history().exec<RemoveRowCmd>(mMap);
}

void MapDocument::remove_column()
{
  get_history().exec<RemoveColumnCmd>(mMap);
}

void MapDocument::resize(const usize rows, const usize cols)
{
  get_history().exec<ResizeMapCmd>(mMap, rows, cols);
}

void MapDocument::fix_tiles()
{
  get_history().exec<FixTilesInMapCmd>(mMap);
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
  get_history().exec<RenameLayerCmd>(mMap, layerId, std::move(name));
}

void MapDocument::move_layer_up(const UUID& layerId)
{
  get_history().exec<MoveLayerUpCmd>(mMap, layerId);
}

void MapDocument::move_layer_down(const UUID& layerId)
{
  get_history().exec<MoveLayerDownCmd>(mMap, layerId);
}

void MapDocument::set_layer_opacity(const UUID& layerId, const float opacity)
{
  auto layer = mMap->get_layer(layerId);
  get_history().exec<SetLayerOpacityCmd>(std::move(layer), opacity);
}

void MapDocument::set_layer_visible(const UUID& layerId, const bool visible)
{
  get_history().exec<SetLayerVisibilityCmd>(mMap, layerId, visible);
}

void MapDocument::register_stamp_sequence(const UUID& layerId,
                                          TileCache   previous,
                                          TileCache   sequence)
{
  get_history().store<StampToolCmd>(mMap,
                                    layerId,
                                    std::move(previous),
                                    std::move(sequence));
}

void MapDocument::register_eraser_sequence(const UUID& layerId, TileCache previous)
{
  get_history().store<EraserToolCmd>(mMap, layerId, std::move(previous));
}

void MapDocument::flood(const UUID&    layerId,
                        const TilePos& origin,
                        const TileID   replacement)
{
  get_history().exec<BucketToolCmd>(mMap, layerId, origin, replacement);
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
  auto object = get_object(objectId);
  get_history().exec<MoveObjectCmd>(std::move(object), previous, updated);
}

void MapDocument::set_object_visible(const UUID& objectId, const bool visible)
{
  auto object = get_object(objectId);
  get_history().exec<SetObjectVisibleCmd>(std::move(object), visible);
}

void MapDocument::set_object_name(const UUID& objectId, std::string name)
{
  auto object = get_object(objectId);
  get_history().exec<SetObjectNameCmd>(std::move(object), std::move(name));
}

void MapDocument::set_object_tag(const UUID& objectId, std::string tag)
{
  auto object = get_object(objectId);
  get_history().exec<SetObjectTagCmd>(std::move(object), std::move(tag));
}

auto MapDocument::get_object(const UUID& objectId) -> Shared<Object>
{
  if (auto ptr = std::dynamic_pointer_cast<Object>(mContexts.get_context(objectId))) {
    return ptr;
  }
  else {
    throw TactileError {"Did not find object!"};
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