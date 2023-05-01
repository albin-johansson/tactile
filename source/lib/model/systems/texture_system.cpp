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

#include "texture_system.hpp"

#include <bit>  // bit_cast

#include "backend/gl/texture.hpp"
#include "common/numeric.hpp"
#include "core/texture.hpp"
#include "io/texture_loader.hpp"

namespace tactile::sys {

// TODO condition on selected backend to determine how to load texture

auto is_texture_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<Texture>(entity) &&
         (model.has<OpenGLTexture>(entity) || model.has<VulkanTexture>(entity));
}

void destroy_loaded_texture_resources(Model& model)
{
  const auto& texture_cache = model.get<TextureCache>();

  for (const auto& [texture_path, texture_entity]: texture_cache.textures) {
    const auto& gl_texture = model.get<OpenGLTexture>(texture_entity);
    gl::destroy_texture(gl_texture.id);
  }
}

auto create_texture(Model& model, const Path& path) -> Entity
{
  auto& texture_cache = model.get<TextureCache>();

  // Check if the image has already been loaded, if so just return the associated entity.
  if (const auto iter = texture_cache.textures.find(path);
      iter != texture_cache.textures.end()) {
    return iter->second;
  }

  const auto texture_data = load_texture_data(path).value();

  const auto texture_entity = model.create_entity();

  auto& texture = model.add<Texture>(texture_entity);
  texture.path = path;
  texture.size = texture_data.size;

  auto& gl_texture = model.add<OpenGLTexture>(texture_entity);
  gl_texture.id = gl::create_texture(texture_data);

  return texture_entity;
}

auto get_texture_handle(const Model& model, const Entity texture_entity) -> void*
{
  TACTILE_ASSERT(is_texture_entity(model, texture_entity));

  const auto& gl_texture = model.get<OpenGLTexture>(texture_entity);
  return std::bit_cast<void*>(static_cast<uintptr>(gl_texture.id));
}

}  // namespace tactile::sys
