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

#include "vk_device.hpp"

#include "backend/vk/vk_common.hpp"
#include "backend/vk/vk_context.hpp"
#include "backend/vk/vk_error.hpp"
#include "backend/vk/vk_gpu.hpp"
#include "common/predef.hpp"
#include "common/primitives.hpp"
#include "common/type/set.hpp"
#include "common/type/vector.hpp"

namespace tactile::vk {

void DeviceDeleter::operator()(VkDevice device) noexcept
{
  vkDestroyDevice(device, nullptr);
}

auto create_device(GPU gpu, VkSurfaceKHR surface) -> UniqueDevice
{
  const auto queue_family_indices = get_queue_family_indices(gpu, surface);

  const auto graphics_family_index = queue_family_indices.graphics_family.value();
  const auto present_family_index = queue_family_indices.present_family.value();

  const Set<uint32> unique_queue_families = {graphics_family_index, present_family_index};

  Vector<VkDeviceQueueCreateInfo> queue_infos;
  queue_infos.reserve(unique_queue_families.size());

  const float queue_priority = 1.0f;

  for (const auto queue_family_index: unique_queue_families) {
    queue_infos.push_back(VkDeviceQueueCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = queue_family_index,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority,
    });
  }

  VkPhysicalDeviceFeatures enabled_device_features {};
  enabled_device_features.samplerAnisotropy = VK_TRUE;
  enabled_device_features.fillModeNonSolid = VK_TRUE;

  VkDeviceCreateInfo device_info {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = nullptr,
      .queueCreateInfoCount = static_cast<uint32>(queue_infos.size()),
      .pQueueCreateInfos = queue_infos.data(),
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = nullptr,
      .enabledExtensionCount = std::size(kRequiredDeviceExtensions),
      .ppEnabledExtensionNames = kRequiredDeviceExtensions,
      .pEnabledFeatures = &enabled_device_features,
  };

  if constexpr (kIsDebugBuild) {
    device_info.enabledLayerCount = std::size(kValidationLayerNames);
    device_info.ppEnabledLayerNames = kValidationLayerNames;
  }

  VkDevice device = VK_NULL_HANDLE;
  if (const auto res = vkCreateDevice(gpu, &device_info, nullptr, &device);
      res != VK_SUCCESS) {
    throw VulkanError {"Could not create Vulkan device", res};
  }

  set_global_device(device);

  return UniqueDevice {device};
}

}  // namespace tactile::vk
