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

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace tactile::vk {

void set_global_instance(VkInstance instance) noexcept;
void set_global_device(VkDevice device) noexcept;
void set_global_allocator(VmaAllocator allocator) noexcept;

[[nodiscard]] auto get_global_instance() noexcept -> VkInstance;
[[nodiscard]] auto get_global_device() noexcept -> VkDevice;
[[nodiscard]] auto get_global_allocator() noexcept -> VmaAllocator ;

}  // namespace tactile::vk
