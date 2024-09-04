// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>   // uint32_t
#include <expected>  // expected

#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

class TACTILE_VULKAN_API VulkanCommandPool final
{
 public:
  TACTILE_DECLARE_MOVE(VulkanCommandPool);
  TACTILE_DELETE_COPY(VulkanCommandPool);

  VulkanCommandPool() = default;

  ~VulkanCommandPool() noexcept;

  VkDevice device {VK_NULL_HANDLE};
  VkCommandPool handle {VK_NULL_HANDLE};

 private:
  void _destroy() noexcept;
};

[[nodiscard]]
TACTILE_VULKAN_API auto create_vulkan_command_pool(VkDevice device,
                                                   std::uint32_t queue_family_index,
                                                   VkCommandPoolCreateFlags flags)
    -> std::expected<VulkanCommandPool, VkResult>;

}  // namespace tactile
