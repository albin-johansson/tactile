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

#include "texture_system_factory.hpp"

#include "common/debug/panic.hpp"
#include "model/textures/gl_texture_system.hpp"
#include "model/textures/null_texture_system.hpp"

namespace tactile {

auto make_texture_system(const BackendAPI api) -> Unique<TextureSystem>
{
  switch (api) {
    case BackendAPI::Null:
      return std::make_unique<NullTextureSystem>();

    case BackendAPI::OpenGL:
      return std::make_unique<OpenGLTextureSystem>();

      // TODO case BackendAPI::Vulkan:
      // TODO   break;

    default:
      throw TactileError {"No texture system available for the specified backend"};
  }
}

}  // namespace tactile
