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

#include "common/debug/panic.hpp"
#include "common/primitives.hpp"

using VkGPU = VkPhysicalDevice;

namespace tactile::vk {

inline constexpr uint32 kVulkanApiVersion = VK_API_VERSION_1_2;

inline constexpr usize kMaxFramesInFlight = 2;

/// Names of the validations layers we use.
inline constexpr const char* kValidationLayerNames[] {
    "VK_LAYER_KHRONOS_validation",
};

/// Names of the extensions that the Vulkan implementation must provide.
inline constexpr const char* kRequiredDeviceExtensions[] {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef TACTILE_USE_VULKAN_SUBSET
    "VK_KHR_portability_subset",
#endif  // TACTILE_USE_VULKAN_SUBSET
};

}  // namespace tactile::vk
