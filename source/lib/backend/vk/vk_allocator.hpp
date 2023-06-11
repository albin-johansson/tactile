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

#include "backend/vk/vk_gpu.hpp"
#include "common/type/ptr.hpp"

namespace tactile::vk {

struct AllocatorDeleter final {
  void operator()(VmaAllocator allocator) noexcept;
};

using UniqueAllocator = Unique<VmaAllocator_T, AllocatorDeleter>;

[[nodiscard]] auto create_allocator(VkInstance instance, GPU gpu, VkDevice device)
    -> UniqueAllocator;

}  // namespace tactile::vk
