// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include <vulkan/vulkan.h>

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

/**
 * Represents a Vulkan texture sampler.
 */
class TACTILE_VULKAN_API VulkanSampler final
{
 public:
  /**
   * Creates a Vulkan sampler from existing resources.
   *
   * \param device  The associated Vulkan device.
   * \param sampler The sampler handle that will be claimed.
   */
  VulkanSampler(VkDevice device, VkSampler sampler) noexcept;

  VulkanSampler(VulkanSampler&& other) noexcept;

  VulkanSampler(const VulkanSampler& other) = delete;

  ~VulkanSampler() noexcept;

  auto operator=(VulkanSampler&& other) noexcept -> VulkanSampler&;

  auto operator=(const VulkanSampler& other) -> VulkanSampler& = delete;

  /**
   * Creates a Vulkan sampler.
   *
   * \param device      The associated Vulkan device.
   * \param create_info The sampler configuration.
   *
   * \return
   * A Vulkan sampler if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto create(VkDevice device, const VkSamplerCreateInfo& create_info)
      -> std::expected<VulkanSampler, VkResult>;

  [[nodiscard]]
  auto device() noexcept -> VkDevice
  {
    return mDevice;
  }

  [[nodiscard]]
  auto get() noexcept -> VkSampler
  {
    return mSampler;
  }

 private:
  VkDevice mDevice {VK_NULL_HANDLE};
  VkSampler mSampler {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
