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

#include "vk_allocator.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "backend/vk/vk_common.hpp"
#include "backend/vk/vk_error.hpp"

namespace tactile::vk {

void AllocatorDeleter::operator()(VmaAllocator allocator) noexcept
{
  vmaDestroyAllocator(allocator);
}

auto create_allocator(VkInstance instance, VkGPU gpu, VkDevice device) -> UniqueAllocator
{
  VmaAllocatorCreateInfo alloc_info {};
  alloc_info.instance = instance;
  alloc_info.physicalDevice = gpu;
  alloc_info.device = device;
  alloc_info.vulkanApiVersion = kVulkanApiVersion;

  VmaAllocator allocator = VK_NULL_HANDLE;
  if (const auto res = vmaCreateAllocator(&alloc_info, &allocator); res != VK_SUCCESS) {
    throw VulkanError {"Could not create allocator", res};
  }

  return UniqueAllocator {allocator};
}

}  // namespace tactile::vk
