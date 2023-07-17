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

#include "texture_factory.hpp"

#include "io/texture_loader.hpp"
#include "model/textures/texture_components.hpp"

namespace tactile::sys {

auto create_texture(Registry& registry, const Path& path) -> Entity
{
  auto& texture_cache = registry.get<TextureCache>();

  // Check if the image has already been loaded, if so just return the associated entity.
  if (const auto iter = texture_cache.textures.find(path);
      iter != texture_cache.textures.end()) {
    return iter->second;
  }

  // TODO return null entity if loading fails
  const auto texture_data = load_texture_data(path).value();

  const auto texture_entity = registry.create_entity();
  texture_cache.textures[path] = texture_entity;

  auto& texture = registry.add<Texture>(texture_entity);
  texture.path = path;
  texture.size = texture_data.size;

  const auto& texture_callbacks = registry.get<TextureCallbacks>();
  texture_callbacks.init(registry, texture_entity, texture_data);

  return texture_entity;
}

}  // namespace tactile::sys
