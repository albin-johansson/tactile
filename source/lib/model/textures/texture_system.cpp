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

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/assert.hpp"
#include "common/util/lookup.hpp"
#include "io/texture_loader.hpp"
#include "model/entity_validation.hpp"
#include "model/textures/texture_components.hpp"

namespace tactile {

void TextureSystem::destroy_textures(Registry& registry)
{
  for (const auto& [texture_path, texture_entity]: mTextureCache) {
    destroy_texture(registry, texture_entity);
  }
}

auto TextureSystem::load_texture(Registry& registry, const Path& texture_path) -> Entity
{
  // Check if the image has already been loaded, if so just return the associated entity.
  if (const auto* texture_entity = find_in(mTextureCache, texture_path)) {
    return *texture_entity;
  }

  const auto texture_data = load_texture_data(texture_path);
  if (!texture_data) {
    spdlog::error("[TextureSystem] Could not load texture {}", texture_path);
    return kNullEntity;
  }

  const auto texture_entity = registry.create_entity();

  auto& texture = registry.add<Texture>(texture_entity);
  texture.path = texture_path;
  texture.size = texture_data->size;

  prepare_texture(registry, texture_entity, *texture_data);
  TACTILE_ASSERT(sys::is_texture_entity(registry, texture_entity));

  mTextureCache[texture_path.string()] = texture_entity;
  return texture_entity;
}

}  // namespace tactile
