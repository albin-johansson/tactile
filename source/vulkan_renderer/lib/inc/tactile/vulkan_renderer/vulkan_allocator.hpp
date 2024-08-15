// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

/**
 * Represents a Vulkan allocator.
 */
class TACTILE_VULKAN_API VulkanAllocator final
{
 public:
  explicit VulkanAllocator(VmaAllocator allocator);

  VulkanAllocator(VulkanAllocator&& other) noexcept;

  VulkanAllocator(const VulkanAllocator&) = delete;

  ~VulkanAllocator() noexcept;

  auto operator=(VulkanAllocator&& other) noexcept -> VulkanAllocator&;

  auto operator=(const VulkanAllocator&) -> VulkanAllocator& = delete;

  /**
   * Creates a Vulkan allocator.
   *
   * \param instance        The associated Vulkan instance.
   * \param physical_device The associated physical device.
   * \param device          The associated logical device.
   *
   * \return
   * A Vulkan allocator if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto create(VkInstance instance,
                     VkPhysicalDevice physical_device,
                     VkDevice device) -> Expected<VulkanAllocator, VkResult>;

 private:
  VmaAllocator mAllocator {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
