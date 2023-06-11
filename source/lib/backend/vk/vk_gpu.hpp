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

#include <vulkan/vulkan.h>

#include "backend/vk/vk_common.hpp"
#include "common/primitives.hpp"
#include "common/type/maybe.hpp"
#include "common/type/vector.hpp"

namespace tactile::vk {

struct QueueFamilyIndices final {
  Maybe<uint32> graphics_family;
  Maybe<uint32> present_family;
};

struct SwapchainSupport final {
  VkSurfaceCapabilitiesKHR capabilities {};
  Vector<VkSurfaceFormatKHR> formats;
  Vector<VkPresentModeKHR> present_modes;
};

[[nodiscard]] auto get_available_gpus() -> Vector<GPU>;

[[nodiscard]] auto get_extensions(GPU gpu) -> Vector<VkExtensionProperties>;

[[nodiscard]] auto get_queue_families(GPU gpu) -> Vector<VkQueueFamilyProperties>;

[[nodiscard]] auto get_queue_family_indices(GPU gpu, VkSurfaceKHR surface)
    -> QueueFamilyIndices;

[[nodiscard]] auto get_surface_formats(GPU gpu, VkSurfaceKHR surface)
    -> Vector<VkSurfaceFormatKHR>;

[[nodiscard]] auto get_present_modes(GPU gpu, VkSurfaceKHR surface)
    -> Vector<VkPresentModeKHR>;

[[nodiscard]] auto get_swapchain_support(GPU gpu, VkSurfaceKHR surface)
    -> SwapchainSupport;

[[nodiscard]] auto get_suitable_gpu(VkSurfaceKHR surface) -> GPU;

}  // namespace tactile::vk
