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

#include "model/services/backend_service.hpp"

namespace tactile {

class GLBackendService final : public BackendService {
 public:
  GLBackendService(SDL_Window* window, SDL_GLContext context);

  ~GLBackendService() noexcept override;

  void process_event(const SDL_Event& event) override;

  [[nodiscard]] auto new_frame() -> Result override;

  void end_frame() override;

  void reload_font_texture() override;

 protected:
  void prepare_texture(Registry& registry,
                       Entity texture_entity,
                       const TextureData& texture_data) override;

  void destroy_texture(void* handle) override;

 private:
  SDL_Window* mWindow {};
};

}  // namespace tactile
