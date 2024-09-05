// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_buffer.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept
  : allocator {std::exchange(other.allocator, VK_NULL_HANDLE)},
    handle {std::exchange(other.handle, VK_NULL_HANDLE)},
    allocation {std::exchange(other.allocation, VK_NULL_HANDLE)}
{}

VulkanBuffer::~VulkanBuffer() noexcept
{
  _destroy();
}

auto VulkanBuffer::operator=(VulkanBuffer&& other) noexcept -> VulkanBuffer&
{
  if (this != &other) {
    _destroy();

    allocator = std::exchange(other.allocator, VK_NULL_HANDLE);
    handle = std::exchange(other.handle, VK_NULL_HANDLE);
    allocation = std::exchange(other.allocation, VK_NULL_HANDLE);
  }

  return *this;
}

void VulkanBuffer::_destroy() noexcept
{
  if (handle != VK_NULL_HANDLE) {
    vmaDestroyBuffer(allocator, handle, allocation);
    handle = VK_NULL_HANDLE;
  }
}

auto create_vulkan_buffer(VmaAllocator allocator,
                          const VkBufferCreateInfo& buffer_info,
                          const VmaAllocationCreateInfo& allocation_info)
    -> std::expected<VulkanBuffer, VkResult>
{
  VulkanBuffer buffer {};
  buffer.allocator = allocator;

  const auto result = vmaCreateBuffer(allocator,
                                      &buffer_info,
                                      &allocation_info,
                                      &buffer.handle,
                                      &buffer.allocation,
                                      nullptr);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan buffer: {}", to_string(result));
    return std::unexpected {result};
  }

  return buffer;
}

}  // namespace tactile
