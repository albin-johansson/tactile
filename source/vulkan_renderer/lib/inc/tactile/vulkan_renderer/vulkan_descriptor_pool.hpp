// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

class TACTILE_VULKAN_API VulkanDescriptorPool final
{
 public:
  TACTILE_DELETE_COPY(VulkanDescriptorPool);
  TACTILE_DECLARE_MOVE(VulkanDescriptorPool);

  VulkanDescriptorPool() noexcept = default;

  ~VulkanDescriptorPool() noexcept;

  VkDevice device {VK_NULL_HANDLE};
  VkDescriptorPool handle {VK_NULL_HANDLE};

 private:
  void _destroy() noexcept;
};

}  // namespace tactile
