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

#include "vk_context.hpp"

namespace tactile::vk {
namespace {

inline VkInstance gInstance = VK_NULL_HANDLE;
inline VkDevice gDevice = VK_NULL_HANDLE;
inline VmaAllocator gAllocator = VK_NULL_HANDLE;

}  // namespace

void set_global_instance(VkInstance instance) noexcept
{
  gInstance = instance;
}

void set_global_device(VkDevice device) noexcept
{
  gDevice = device;
}

void set_global_allocator(VmaAllocator allocator) noexcept
{
  gAllocator = allocator;
}

auto get_global_instance() noexcept -> VkInstance
{
  return gInstance;
}

auto get_global_device() noexcept -> VkDevice
{
  return gDevice;
}

auto get_global_allocator() noexcept -> VmaAllocator
{
  return gAllocator;
}

}  // namespace tactile::vk
