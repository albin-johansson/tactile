// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

/**
 * Represents a Vulkan fence synchronization primitive.
 */
class TACTILE_VULKAN_API VulkanFence final
{
 public:
  TACTILE_DECLARE_MOVE(VulkanFence);
  TACTILE_DELETE_COPY(VulkanFence);

  VulkanFence() = default;

  /**
   * Creates a Vulkan fence from existing resources.
   *
   * \param device The associated Vulkan device.
   * \param fence  The fence handle that will be claimed.
   */
  VulkanFence(VkDevice device, VkFence fence);

  ~VulkanFence() noexcept;

  /**
   * Creates a Vulkan fence.
   *
   * \param device The associated Vulkan device.
   * \param flags  The fence creation flags.
   *
   * \return
   * A Vulkan fence if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto create(VkDevice device,
                     VkFenceCreateFlags flags) -> std::expected<VulkanFence, VkResult>;

  [[nodiscard]]
  auto reset() -> VkResult;

  [[nodiscard]]
  auto wait() -> VkResult;

  [[nodiscard]]
  auto device() -> VkDevice;

  [[nodiscard]]
  auto get() -> VkFence;

 private:
  VkDevice m_device {VK_NULL_HANDLE};
  VkFence m_fence {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
