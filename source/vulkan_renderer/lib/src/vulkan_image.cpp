// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_image.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanImage::VulkanImage(VulkanImage&& other) noexcept
    : allocator {std::exchange(other.allocator, VK_NULL_HANDLE)},
      handle {std::exchange(other.handle, VK_NULL_HANDLE)},
      allocation {std::exchange(other.allocation, VK_NULL_HANDLE)},
      params {std::exchange(other.params, VulkanImageParams {})}
{}

VulkanImage::~VulkanImage() noexcept
{
  _destroy();
}

auto VulkanImage::operator=(VulkanImage&& other) noexcept -> VulkanImage&
{
  if (this != &other) {
    _destroy();

    allocator = std::exchange(other.allocator, VK_NULL_HANDLE);
    handle = std::exchange(other.handle, VK_NULL_HANDLE);
    allocation = std::exchange(other.allocation, VK_NULL_HANDLE);
    params = std::exchange(other.params, VulkanImageParams {});
  }

  return *this;
}

void VulkanImage::_destroy() noexcept
{
  if (handle != VK_NULL_HANDLE) {
    vmaDestroyImage(allocator, handle, allocation);
    handle = VK_NULL_HANDLE;
  }
}

auto create_vulkan_image(VmaAllocator allocator, const VulkanImageParams& params)
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

  VulkanImage image {};
  image.allocator = allocator;
  image.params = params;

  const auto result = vmaCreateImage(allocator,
                                     &image_info,
                                     &allocation_info,
                                     &image.handle,
                                     &image.allocation,
                                     nullptr);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan image: {}", to_string(result));
    return std::unexpected {result};
  }

  return image;
}

}  // namespace tactile
