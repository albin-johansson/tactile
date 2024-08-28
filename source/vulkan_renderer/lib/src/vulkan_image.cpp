// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_image.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanImage::VulkanImage(VmaAllocator allocator,
                         VkImage image,
                         VmaAllocation allocation,
                         const VulkanImageParams& params)
    : m_allocator {allocator},
      m_image {image},
      m_allocation {allocation},
      m_params {params}
{}

VulkanImage::VulkanImage(VulkanImage&& other) noexcept
    : m_allocator {std::exchange(other.m_allocator, VK_NULL_HANDLE)},
      m_image {std::exchange(other.m_image, VK_NULL_HANDLE)},
      m_allocation {std::exchange(other.m_allocation, VK_NULL_HANDLE)},
      m_params {std::exchange(other.m_params, VulkanImageParams {})}
{}

VulkanImage::~VulkanImage() noexcept
{
  _destroy();
}

auto VulkanImage::operator=(VulkanImage&& other) noexcept -> VulkanImage&
{
  if (this != &other) {
    _destroy();

    m_allocator = std::exchange(other.m_allocator, VK_NULL_HANDLE);
    m_image = std::exchange(other.m_image, VK_NULL_HANDLE);
    m_allocation = std::exchange(other.m_allocation, VK_NULL_HANDLE);
    m_params = std::exchange(other.m_params, VulkanImageParams {});
  }

  return *this;
}

void VulkanImage::_destroy() noexcept
{
  if (m_image != VK_NULL_HANDLE) {
    vmaDestroyImage(m_allocator, m_image, m_allocation);
    m_image = VK_NULL_HANDLE;
  }
}

auto VulkanImage::create(VmaAllocator allocator, const VulkanImageParams& params)
    -> std::expected<VulkanImage, VkResult>
{
  const VkImageCreateInfo image_info {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .imageType = params.type,
    .format = params.format,
    .extent = VkExtent3D {params.extent.width, params.extent.height, 1},
    .mipLevels = params.mip_levels,
    .arrayLayers = 1,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .tiling = VK_IMAGE_TILING_OPTIMAL,
    .usage = params.usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = nullptr,
    .initialLayout = params.layout,
  };

  constexpr VmaAllocationCreateInfo allocation_info {
    .flags = 0,
    .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    .requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    .preferredFlags = 0,
    .memoryTypeBits = 0,
    .pool = nullptr,
    .pUserData = nullptr,
    .priority = 0.0f,
  };

  VkImage image {};
  VmaAllocation allocation {};
  const auto result =
      vmaCreateImage(allocator, &image_info, &allocation_info, &image, &allocation, nullptr);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan image: {}", to_string(result));
    return std::unexpected {result};
  }

  return VulkanImage {allocator, image, allocation, params};
}

}  // namespace tactile
