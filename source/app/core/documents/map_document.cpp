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
#include "core/commands/maps/add_column_cmd.hpp"
#include "core/commands/maps/add_row_cmd.hpp"
#include "core/commands/maps/fix_tiles_in_map_cmd.hpp"
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
  sys::update_tilesets(mRegistry);
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

void MapDocument::fix_tiles()
{
  get_history().push<FixTilesInMapCmd>(this);
}

auto MapDocument::is_tool_active(const ToolType tool) const -> bool
{
  return get_tools().is_enabled(tool);
}

auto MapDocument::is_tool_possible(const ToolType tool) const -> bool
{
  return get_tools().is_available(mRegistry, tool);
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