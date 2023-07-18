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

#pragma once

#include "common/macros.hpp"
#include "common/type/ecs.hpp"
#include "common/type/path.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "model/registry.hpp"
#include "model/system.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(TextureData)

class TextureSystem : public System {
 public:
  void destroy_textures(Registry& registry);

  [[nodiscard]] auto load_texture(Registry& registry, const Path& texture_path) -> Entity;

 protected:
  virtual void prepare_texture(Registry& registry,
                               Entity texture_entity,
                               const TextureData& texture_data) = 0;

  virtual void destroy_texture(Registry& registry, Entity texture_entity) = 0;

 private:
  StringMap<Entity> mTextureCache;
};

}  // namespace tactile
