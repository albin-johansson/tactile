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

#include "null_texture_system.hpp"

#include "model/textures/texture_components.hpp"

namespace tactile::sys {

void on_init_null_texture(Registry& registry,
                          const Entity texture_entity,
                          const TextureData&)
{
  registry.add<NullTexture>(texture_entity);
}

void on_destroy_null_texture(Registry&, Entity)
{
  // Do nothing
}

}  // namespace tactile::sys
