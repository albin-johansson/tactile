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

#include <SDL2/SDL.h>

#include "common/result.hpp"
#include "common/type/ecs.hpp"
#include "common/type/path.hpp"
#include "core/containers/string_map.hpp"
#include "core/prelude.hpp"
#include "model/registry.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(TextureData)

class BackendService {
 public:
  virtual ~BackendService() noexcept = default;

  virtual void process_event(const SDL_Event& event) = 0;

  /**
   * Attempts to begin a new frame.
   *
   * \return `success` if rendering can proceed; `failure` otherwise.
   */
  [[nodiscard]] virtual auto new_frame() -> Result = 0;

  /**
   * Ends the current frame.
   *
   * \note This function can only be called after a successful call to `new_frame`.
   */
  virtual void end_frame() = 0;

  virtual void reload_font_texture() {}

  [[nodiscard]] auto load_texture(Registry& registry, const Path& texture_path) -> Entity;

  void destroy_texture(Registry& registry, Entity texture_entity);

  void destroy_all_textures(Registry& registry);

 protected:
  virtual void prepare_texture(Registry& registry,
                               Entity texture_entity,
                               const TextureData& texture_data) = 0;

  virtual void destroy_texture(void* handle) = 0;

 private:
  StringMap<Entity> mTextureCache;
};

}  // namespace tactile
