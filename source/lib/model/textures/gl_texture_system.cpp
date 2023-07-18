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

#include "gl_texture_system.hpp"

#include <bit>  // bit_cast

#include "backend/gl/gl_texture.hpp"
#include "common/primitives.hpp"
#include "model/textures/texture_components.hpp"

namespace tactile {

void OpenGLTextureSystem::prepare_texture(Registry& registry,
                                          const Entity texture_entity,
                                          const TextureData& texture_data)
{
  auto& gl_texture = registry.add<OpenGLTexture>(texture_entity);
  gl_texture.id = gl::create_texture(texture_data);

  auto& texture = registry.get<Texture>(texture_entity);
  texture.handle = std::bit_cast<void*>(static_cast<uintptr>(gl_texture.id));
}

void OpenGLTextureSystem::destroy_texture(Registry& registry, const Entity texture_entity)
{
  const auto& gl_texture = registry.get<OpenGLTexture>(texture_entity);
  gl::destroy_texture(gl_texture.id);
}

}  // namespace tactile::sys