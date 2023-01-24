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

#include "icons.hpp"

#include "common/debug/assert.hpp"
#include "common/debug/panic.hpp"
#include "common/type/ptr.hpp"
#include "core/texture.hpp"
#include "io/directories.hpp"

namespace tactile::ui {
namespace {

inline Shared<Texture> gTactileIcon;

}  // namespace

void load_icons()
{
  gTactileIcon = io::load_texture(find_resource("assets/icon.png"));
}

void unload_icons() noexcept
{
  gTactileIcon.reset();
}

auto get_icon(const LayerType type) -> const char*
{
  switch (type) {
    case LayerType::TileLayer:
      return TAC_ICON_TILE_LAYER;

    case LayerType::ObjectLayer:
      return TAC_ICON_OBJECT_LAYER;

    case LayerType::GroupLayer:
      return TAC_ICON_GROUP_LAYER;

    default:
      throw TactileError {"Invalid layer type!"};
  }
}

auto get_icon(const ObjectType type) -> const char*
{
  switch (type) {
    case ObjectType::Rect:
      return TAC_ICON_RECTANGLE;

    case ObjectType::Ellipse:
      return TAC_ICON_ELLIPSE;

    case ObjectType::Point:
      return TAC_ICON_POINT;

    default:
      throw TactileError {"Invalid object type!"};
  }
}

auto get_tactile_icon() -> uint
{
  TACTILE_ASSERT(gTactileIcon != nullptr);
  return gTactileIcon->get_id();
}

}  // namespace tactile::ui
