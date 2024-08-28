// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>  // expected
#include <memory>    // unique_ptr

#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

class IWindow;

struct TACTILE_VULKAN_API VulkanInstanceDeleter final
{
  void operator()(VkInstance instance) noexcept;
};

using VulkanInstance = std::unique_ptr<VkInstance_T, VulkanInstanceDeleter>;

/**
 * Creates a Vulkan instance.
 *
 * \param window The target window.
 *
 * \return
 * A Vulkan instance if successful; an error code otherwise.
 */
[[nodiscard]]
TACTILE_VULKAN_API auto create_vulkan_instance(IWindow& window)
    -> std::expected<VulkanInstance, VkResult>;

}  // namespace tactile
