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

#include "registry_system.hpp"

#include "core/commands/command_stack.hpp"
#include "core/components/attributes.hpp"
#include "core/components/contexts.hpp"
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/components/objects.hpp"
#include "core/components/tiles.hpp"
#include "core/components/tools.hpp"
#include "core/components/viewport.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/tools/tool_manager.hpp"

namespace tactile::sys {

auto new_map_document_registry() -> entt::registry
{
  entt::registry registry;

  auto& ctx = registry.ctx();
  ctx.emplace<comp::ActiveLayer>();
  ctx.emplace<comp::ActiveTileset>();
  ctx.emplace<comp::ActiveContext>();
  ctx.emplace<comp::ActiveObject>();
  ctx.emplace<comp::ContextMapping>();
  ctx.emplace<ToolManager>();
  ctx.emplace<CommandStack>();

  auto& map = ctx.emplace<comp::MapInfo>();
  map.row_count = 5;
  map.column_count = 5;

  auto& viewport = ctx.emplace<comp::Viewport>();
  viewport.offset = {0, 0};
  viewport.tile_size = {64, 64};

  auto& context = ctx.emplace<comp::Context>();
  context.id = make_uuid();
  context.name = "Map";

  auto& tilesets = ctx.emplace<comp::TilesetContext>();
  tilesets.next_tile_id = 1;

  return registry;
}

}  // namespace tactile::sys
