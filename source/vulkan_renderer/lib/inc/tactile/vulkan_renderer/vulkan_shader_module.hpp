// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vulkan/vulkan.h>

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

/**
 * Represents a Vulkan shader module.
 */
class TACTILE_VULKAN_API VulkanShaderModule final
{
 public:
  /**
   * Creates a Vulkan shader module from existing resources.
   *
   * \param device        The associated Vulkan device.
   * \param shader_module The shader module handle that will be claimed.
   */
  VulkanShaderModule(VkDevice device, VkShaderModule shader_module) noexcept;

  VulkanShaderModule(VulkanShaderModule&& other) noexcept;

  VulkanShaderModule(const VulkanShaderModule& other) = delete;

  ~VulkanShaderModule() noexcept;

  auto operator=(VulkanShaderModule&& other) noexcept -> VulkanShaderModule&;

  auto operator=(const VulkanShaderModule& other) -> VulkanShaderModule& = delete;

  /**
   * Creates a Vulkan shader module from a compiled shader.
   *
   * \param device      The associated Vulkan device.
   * \param shader_path The file path of the binary shader file.
   *
   * \return
   * A Vulkan shader module if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto load(VkDevice device,
                   const Path& shader_path) -> Expected<VulkanShaderModule, VkResult>;

  [[nodiscard]]
  auto device() noexcept -> VkDevice
  {
    return mDevice;
  }

  [[nodiscard]]
  auto get() noexcept -> VkShaderModule
  {
    return mShaderModule;
  }

 private:
  VkDevice mDevice {VK_NULL_HANDLE};
  VkShaderModule mShaderModule {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
