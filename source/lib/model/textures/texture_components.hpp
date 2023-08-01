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

#include "core/ecs/entity.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "core/containers/tree_map.hpp"
#include "core/prelude.hpp"
#include "model/registry.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(TextureData)

/// Context component tracking loaded icon textures.
struct Icons final {
  Entity tactile_icon {kNullEntity};
};

/**
 * Component describing a loaded texture.
 *
 * \details The actual type of the handle member depends on the backend API that is being
 *          used. For OpenGL, the handle should be reinterpreted as a texture identifier.
 *          For Vulkan, the handle is a VkDescriptorSet. This is dictated by the
 *          associated Dear ImGui backend implementations.
 */
struct Texture final {
  void* handle {};  ///< Opaque handle to the backend texture resource.
  Int2 size {};     ///< The texture dimensions.
  Path path;        ///< Path to the file from which the texture was created.
};

/// Component featured by texture entities loaded using the null backend.
struct NullTexture final {
  char unused;
};

/// Component featured by OpenGL texture entities.
struct OpenGLTexture final {
  uint id {};  ///< Associated OpenGL texture ID.
};

/// Component featured by Vulkan texture entities.
struct VulkanTexture final {
  // TODO VkImage image {};
  // TODO VkImageView view {};
  // TODO VkDescriptorSet imgui_descriptor_set {};
};

}  // namespace tactile
