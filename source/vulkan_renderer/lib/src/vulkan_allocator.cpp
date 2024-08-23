// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_allocator.hpp"

#include <utility>  // exchange

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanAllocator::VulkanAllocator(VmaAllocator allocator) :
  mAllocator {allocator}
{}

VulkanAllocator::VulkanAllocator(VulkanAllocator&& other) noexcept :
  mAllocator {std::exchange(other.mAllocator, VK_NULL_HANDLE)}
{}

VulkanAllocator::~VulkanAllocator() noexcept
{
  _destroy();
}

void VulkanAllocator::_destroy() noexcept
{
  if (mAllocator != VK_NULL_HANDLE) {
    vmaDestroyAllocator(mAllocator);
    mAllocator = VK_NULL_HANDLE;
  }
}

auto VulkanAllocator::operator=(VulkanAllocator&& other) noexcept -> VulkanAllocator&
{
  if (this != &other) {
    _destroy();

    mAllocator = std::exchange(other.mAllocator, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanAllocator::create(VkInstance instance,
                             VkPhysicalDevice physical_device,
                             VkDevice device) -> std::expected<VulkanAllocator, VkResult>
{
  const VmaAllocatorCreateInfo create_info {
    .flags = 0,
    .physicalDevice = physical_device,
    .device = device,
    .preferredLargeHeapBlockSize = 0,
    .pAllocationCallbacks = nullptr,
    .pDeviceMemoryCallbacks = nullptr,
    .pHeapSizeLimit = nullptr,
    .pVulkanFunctions = nullptr,
    .instance = instance,
    .vulkanApiVersion = VK_API_VERSION_1_2,
    .pTypeExternalMemoryHandleTypes = nullptr,
  };

  VmaAllocator allocator {};
  const auto result = vmaCreateAllocator(&create_info, &allocator);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan allocator: {}", to_string(result));
    return std::unexpected {result};
  }

  return VulkanAllocator {allocator};
}

}  // namespace tactile
