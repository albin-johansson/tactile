// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vulkan/vulkan.h>

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

/**
 * Represents a Vulkan semaphore synchronization primitive.
 */
class TACTILE_VULKAN_API VulkanSemaphore final
{
 public:
  /**
   * Creates a Vulkan semaphore from existing resources.
   *
   * \param device    The associated Vulkan device.
   * \param semaphore The semaphore handle that will be claimed.
   */
  VulkanSemaphore(VkDevice device, VkSemaphore semaphore);

  VulkanSemaphore(VulkanSemaphore&& other) noexcept;

  VulkanSemaphore(const VulkanSemaphore&) = delete;

  ~VulkanSemaphore() noexcept;

  auto operator=(VulkanSemaphore&& other) noexcept -> VulkanSemaphore&;

  auto operator=(const VulkanSemaphore&) -> VulkanSemaphore& = delete;

  /**
   * Creates a Vulkan semaphore.
   *
   * \param device The associated Vulkan device.
   *
   * \return
   * A Vulkan semaphore if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto create(VkDevice device) -> Expected<VulkanSemaphore, VkResult>;

  [[nodiscard]]
  auto device() noexcept -> VkDevice
  {
    return mDevice;
  }

  [[nodiscard]]
  auto get() -> VkSemaphore
  {
    return mSemaphore;
  }

 private:
  VkDevice mDevice {VK_NULL_HANDLE};
  VkSemaphore mSemaphore {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
