// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

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
  /**
   * Creates a Vulkan fence from existing resources.
   *
   * \param device The associated Vulkan device.
   * \param fence  The fence handle that will be claimed.
   */
  VulkanFence(VkDevice device, VkFence fence);

  VulkanFence(VulkanFence&& other) noexcept;

  VulkanFence(const VulkanFence&) = delete;

  ~VulkanFence() noexcept;

  auto operator=(VulkanFence&& other) noexcept -> VulkanFence&;

  auto operator=(const VulkanFence&) -> VulkanFence& = delete;

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
  auto device() noexcept -> VkDevice
  {
    return mDevice;
  }

  [[nodiscard]]
  auto get() noexcept -> VkFence
  {
    return mFence;
  }

 private:
  VkDevice mDevice {VK_NULL_HANDLE};
  VkFence mFence {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
