// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected

#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

/**
 * Represents a Vulkan texture sampler.
 */
class TACTILE_VULKAN_API VulkanSampler final
{
 public:
  TACTILE_DECLARE_MOVE(VulkanSampler);
  TACTILE_DELETE_COPY(VulkanSampler);

  VulkanSampler() = default;

  /**
   * Creates a Vulkan sampler from existing resources.
   *
   * \param device  The associated Vulkan device.
   * \param sampler The sampler handle that will be claimed.
   */
  VulkanSampler(VkDevice device, VkSampler sampler) noexcept;

  ~VulkanSampler() noexcept;

  /**
   * Creates a Vulkan sampler.
   *
   * \param device The associated Vulkan device.
   *
   * \return
   * A Vulkan sampler if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto create(VkDevice device) -> std::expected<VulkanSampler, VkResult>;

  [[nodiscard]]
  auto device() -> VkDevice;

  [[nodiscard]]
  auto get() -> VkSampler;

 private:
  VkDevice m_device {VK_NULL_HANDLE};
  VkSampler m_sampler {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
