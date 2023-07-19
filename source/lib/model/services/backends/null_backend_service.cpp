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

#include "null_backend_service.hpp"

namespace tactile {

void NullBackendService::process_event(const SDL_Event&) {}

auto NullBackendService::new_frame() -> Result
{
  return failure;
}

void NullBackendService::end_frame() {}

void NullBackendService::prepare_texture(Registry&, Entity, const TextureData&) {}

void NullBackendService::destroy_texture(void*) {}

}  // namespace tactile
