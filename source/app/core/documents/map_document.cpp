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

#include <utility>  // move

#include "core/commands/command_stack.hpp"
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
#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/map_info.hpp"
#include "core/systems/animation_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tilesets/tileset_system.hpp"
#include "core/tools/tool_manager.hpp"

namespace tactile {

MapDocument::MapDocument(const Vector2i& tileSize, const usize rows, const usize columns)
    : ADocument{sys::new_map_document_registry()}
{
  auto& info = ctx_get<comp::MapInfo>(mRegistry);
  info.tile_size = tileSize;
  info.row_count = rows;
  info.column_count = columns;

  auto& context = ctx_get<comp::AttributeContext>(mRegistry);
  context.name = "Map";
}

void MapDocument::update()
{
  sys::update_animations(mRegistry);
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

void MapDocument::move_object(const UUID& objectId,
                              const Vector2f& previous,
                              const Vector2f& updated)
{
  get_history().push<MoveObjectCmd>(this, objectId, previous, updated);
}

auto MapDocument::info() const -> const comp::MapInfo&
{
  return ctx_get<comp::MapInfo>(mRegistry);
}

auto MapDocument::viewport() const -> const comp::Viewport&
{
  return ctx_get<comp::Viewport>(mRegistry);
}

auto MapDocument::tile_size() const -> Vector2i
{
  const auto& info = ctx_get<comp::MapInfo>(mRegistry);
  return info.tile_size;
}

auto MapDocument::get_tools() -> ToolManager&
{
  return ctx_get<ToolManager>(mRegistry);
}

auto MapDocument::get_tools() const -> const ToolManager&
{
  return ctx_get<ToolManager>(mRegistry);
}

}  // namespace tactile