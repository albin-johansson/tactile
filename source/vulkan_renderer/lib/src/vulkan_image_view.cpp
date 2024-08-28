// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_image_view.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanImageView::VulkanImageView(VkDevice device, VkImageView image_view)
    : m_device {device},
      m_image_view {image_view}
{}

VulkanImageView::~VulkanImageView() noexcept
{
  _destroy();
}

VulkanImageView::VulkanImageView(VulkanImageView&& other) noexcept
    : m_device {std::exchange(other.m_device, VK_NULL_HANDLE)},
      m_image_view {std::exchange(other.m_image_view, VK_NULL_HANDLE)}
{}

auto VulkanImageView::operator=(VulkanImageView&& other) noexcept -> VulkanImageView&
{
  if (this != &other) {
    _destroy();

    m_device = std::exchange(other.m_device, VK_NULL_HANDLE);
    m_image_view = std::exchange(other.m_image_view, VK_NULL_HANDLE);
  }

  return *this;
}

void VulkanImageView::_destroy() noexcept
{
  if (m_image_view != VK_NULL_HANDLE) {
    vkDestroyImageView(m_device, m_image_view, nullptr);
    m_image_view = VK_NULL_HANDLE;
  }
}

auto VulkanImageView::create(VkDevice device,
                             VkImage image,
                             const VkFormat format,
                             const VkImageViewType type,
                             const VkImageAspectFlags aspect_mask,
                             const std::uint32_t mip_levels)
    -> std::expected<VulkanImageView, VkResult>
{
  const VkImageViewCreateInfo image_view_info {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .image = image,
    .viewType = type,
    .format = format,
    .components =
        VkComponentMapping {
          .r = VK_COMPONENT_SWIZZLE_IDENTITY,
          .g = VK_COMPONENT_SWIZZLE_IDENTITY,
          .b = VK_COMPONENT_SWIZZLE_IDENTITY,
          .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
    .subresourceRange =
        VkImageSubresourceRange {
          .aspectMask = aspect_mask,
          .baseMipLevel = 0,
          .levelCount = mip_levels,
          .baseArrayLayer = 0,
          .layerCount = 1,
        },
  };

  VkImageView image_view {};
  const auto result = vkCreateImageView(device, &image_view_info, nullptr, &image_view);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan image view: {}", to_string(result));
    return std::unexpected {result};
  }

  return VulkanImageView {device, image_view};
}

}  // namespace tactile
