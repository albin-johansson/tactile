// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vulkan/vulkan.h>

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

class IWindow;

/**
 * Represents a Vulkan surface.
 */
class TACTILE_VULKAN_API VulkanSurface final
{
 public:
  VulkanSurface(VkInstance instance, VkSurfaceKHR surface);

  VulkanSurface(VulkanSurface&& other) noexcept;

  VulkanSurface(const VulkanSurface&) = delete;

  ~VulkanSurface() noexcept;

  auto operator=(VulkanSurface&& other) noexcept -> VulkanSurface&;

  auto operator=(const VulkanSurface&) -> VulkanSurface& = delete;

  /**
   * Creates a Vulkan surface.
   *
   * \param instance The associated Vulkan instance.
   * \param window   The target window.
   *
   * \return
   * A Vulkan surface if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto create(VkInstance instance,
                     IWindow* window) -> Expected<VulkanSurface, VkResult>;

  [[nodiscard]]
  auto instance() noexcept -> VkInstance
  {
    return mInstance;
  }

  [[nodiscard]]
  auto get() noexcept -> VkSurfaceKHR
  {
    return mSurface;
  }

 private:
  VkInstance mInstance {VK_NULL_HANDLE};
  VkSurfaceKHR mSurface {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
