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

#include "icons.hpp"

#include "core/common/maybe.hpp"
#include "io/directories.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

constinit Maybe<uint> _tactile_icon;

}  // namespace

void load_icons(TextureManager& textures)
{
  _tactile_icon = textures.load(find_resource("assets/icon.png")).value().id;
}

auto get_tactile_icon() -> uint
{
  TACTILE_ASSERT(_tactile_icon.has_value());
  return _tactile_icon.value();
}

}  // namespace tactile
