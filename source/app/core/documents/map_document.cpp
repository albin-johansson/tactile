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

#include "core/commands/layers/add_layer_cmd.hpp"
#include "core/commands/layers/duplicate_layer_cmd.hpp"
#include "core/commands/layers/move_layer_down_cmd.hpp"
#include "core/commands/layers/move_layer_up_cmd.hpp"
#include "core/commands/layers/remove_layer_cmd.hpp"
#include "core/commands/layers/rename_layer_cmd.hpp"
#include "core/commands/layers/set_layer_opacity_cmd.hpp"
#include "core/commands/layers/set_layer_visibility_cmd.hpp"
#include "core/commands/maps/add_column_cmd.hpp"
#include "core/commands/maps/add_row_cmd.hpp"
#include "core/commands/maps/fix_tiles_in_map_cmd.hpp"
#include "core/commands/maps/remove_column_cmd.hpp"
#include "core/commands/maps/remove_row_cmd.hpp"
#include "core/commands/maps/resize_map_cmd.hpp"
#include "core/commands/objects/move_object_cmd.hpp"
#include "core/commands/objects/set_object_visible_cmd.hpp"
#include "core/commands/tools/bucket_tool_cmd.hpp"
#include "core/commands/tools/ellipse_tool_cmd.hpp"
#include "core/commands/tools/eraser_tool_cmd.hpp"
#include "core/commands/tools/point_tool_cmd.hpp"
#include "core/commands/tools/rectangle_tool_cmd.hpp"
#include "core/commands/tools/stamp_tool_cmd.hpp"

namespace tactile {

MapDocument::MapDocument(const Vector2i& tileSize, const usize rows, const usize columns)
{
  mMap.resize(rows, columns);
  mMap.set_tile_size(tileSize);
}

void MapDocument::update()
{
  // TODO sys::update_animations(mRegistry);
}

void MapDocument::add_row()
{
  get_history().push<AddRowCmd>(this);
}

void MapDocument::add_column()
{
  get_history().push<AddColumnCmd>(this);
}

void MapDocument::remove_row()
{
  get_history().push<RemoveRowCmd>(this);
}

void MapDocument::remove_column()
{
  get_history().push<RemoveColumnCmd>(this);
}

void MapDocument::resize(const usize rows, const usize cols)
{
  get_history().push<ResizeMapCmd>(this, rows, cols);
}

void MapDocument::fix_tiles()
{
  get_history().push<FixTilesInMapCmd>(this);
}

void MapDocument::add_layer(const LayerType type)
{
  get_history().push<AddLayerCmd>(this, type);
}

void MapDocument::remove_layer(const UUID& layerId)
{
  get_history().push<RemoveLayerCmd>(this, layerId);
}

void MapDocument::duplicate_layer(const UUID& layerId)
{
  get_history().push<DuplicateLayerCmd>(this, layerId);
}

void MapDocument::rename_layer(const UUID& layerId, std::string name)
{
  get_history().push<RenameLayerCmd>(this, layerId, std::move(name));
}

void MapDocument::move_layer_up(const UUID& layerId)
{
  get_history().push<MoveLayerUpCmd>(this, layerId);
}

void MapDocument::move_layer_down(const UUID& layerId)
{
  get_history().push<MoveLayerDownCmd>(this, layerId);
}

void MapDocument::set_layer_opacity(const UUID& layerId, const float opacity)
{
  get_history().push<SetLayerOpacityCmd>(this, layerId, opacity);
}

void MapDocument::set_layer_visible(const UUID& layerId, const bool visible)
{
  get_history().push<SetLayerVisibilityCmd>(this, layerId, visible);
}

void MapDocument::register_stamp_sequence(const UUID& layerId,
                                          TileCache   previous,
                                          TileCache   sequence)
{
  get_history().push_without_redo<StampToolCmd>(this,
                                                layerId,
                                                std::move(previous),
                                                std::move(sequence));
}

void MapDocument::register_eraser_sequence(const UUID& layerId, TileCache previous)
{
  get_history().push_without_redo<EraserToolCmd>(this, layerId, std::move(previous));
}

void MapDocument::flood(const UUID&    layerId,
                        const TilePos& origin,
                        const TileID   replacement)
{
  get_history().push<BucketToolCmd>(this, layerId, origin, replacement);
}

void MapDocument::add_rectangle(const UUID&     layerId,
                                const Vector2f& pos,
                                const Vector2f& size)
{
  get_history().push<RectangleToolCmd>(this, layerId, pos, size);
}

void MapDocument::add_ellipse(const UUID&     layerId,
                              const Vector2f& pos,
                              const Vector2f& size)
{
  get_history().push<EllipseToolCmd>(this, layerId, pos, size);
}

void MapDocument::add_point(const UUID& layerId, const Vector2f& pos)
{
  get_history().push<PointToolCmd>(this, layerId, pos);
}

void MapDocument::move_object(const UUID&     layerId,
                              const UUID&     objectId,
                              const Vector2f& previous,
                              const Vector2f& updated)
{
  get_history().push<MoveObjectCmd>(this, layerId, objectId, previous, updated);
}

void MapDocument::set_object_visible(const UUID& layerId,
                                     const UUID& objectId,
                                     const bool  visible)
{
  get_history().push<SetObjectVisibleCmd>(this, layerId, objectId, visible);
}

void MapDocument::set_name(std::string name)
{
  mMap.set_name(name);
}

auto MapDocument::get_name() const -> const std::string&
{
  return mMap.get_name();
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