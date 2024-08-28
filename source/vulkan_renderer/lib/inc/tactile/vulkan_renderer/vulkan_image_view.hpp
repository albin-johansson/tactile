// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>   // uint32_t
#include <expected>  // expected

#include <vulkan/vulkan.h>

#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

class TACTILE_VULKAN_API VulkanImageView final
{
 public:
  TACTILE_DELETE_COPY(VulkanImageView);
  TACTILE_DECLARE_MOVE(VulkanImageView);

  VulkanImageView() = default;

  VulkanImageView(VkDevice device, VkImageView image_view);

  ~VulkanImageView() noexcept;

  [[nodiscard]]
  static auto create(VkDevice device,
                     VkImage image,
                     VkFormat format,
                     VkImageViewType type,
                     VkImageAspectFlags aspect_mask,
                     std::uint32_t mip_levels) -> std::expected<VulkanImageView, VkResult>;

  [[nodiscard]]
  auto device() noexcept -> VkDevice
  {
    return m_device;
  }

  [[nodiscard]]
  auto get() noexcept -> VkImageView
  {
    return m_image_view;
  }

 private:
  VkDevice m_device {VK_NULL_HANDLE};
  VkImageView m_image_view {VK_NULL_HANDLE};

  void _destroy() noexcept;
};

}  // namespace tactile
