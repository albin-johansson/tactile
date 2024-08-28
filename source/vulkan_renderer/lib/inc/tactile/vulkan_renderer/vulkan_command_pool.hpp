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

  VulkanCommandPool(VkDevice device, VkCommandPool pool);

  ~VulkanCommandPool() noexcept;

  [[nodiscard]]
  static auto create(VkDevice device,
                     std::uint32_t queue_family_index,
                     VkCommandPoolCreateFlags flags)
      -> std::expected<VulkanCommandPool, VkResult>;

  [[nodiscard]]
  auto device() -> VkDevice;

  [[nodiscard]]
  auto get() -> VkCommandPool;

 private:
  VkDevice m_device {VK_NULL_HANDLE};
  VkCommandPool m_pool {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
