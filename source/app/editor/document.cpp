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

#include "document.hpp"

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/map_info.hpp"
#include "core/components/tiles.hpp"

namespace tactile {

auto Document::is_map() const -> bool
{
  return registry.ctx().contains<MapInfo>();
}

auto Document::is_tileset() const -> bool
{
  return registry.ctx().contains<comp::Tileset>();
}

auto Document::id() const -> const UUID&
{
  return ctx_get<comp::AttributeContext>(registry).id;
}

}  // namespace tactile