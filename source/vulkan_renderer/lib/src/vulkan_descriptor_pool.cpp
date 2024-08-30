// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_descriptor_pool.hpp"

#include <utility>  // exchange

namespace tactile {

VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool&& other) noexcept
    : device {std::exchange(other.device, VK_NULL_HANDLE)},
      handle {std::exchange(other.handle, VK_NULL_HANDLE)}
{}

VulkanDescriptorPool::~VulkanDescriptorPool() noexcept
{
  _destroy();
}

auto VulkanDescriptorPool::operator=(VulkanDescriptorPool&& other) noexcept
    -> VulkanDescriptorPool&
{
  if (this != &other) {
    _destroy();

    device = std::exchange(other.device, VK_NULL_HANDLE);
    handle = std::exchange(other.handle, VK_NULL_HANDLE);
  }

  return *this;
}

void VulkanDescriptorPool::_destroy() noexcept
{
  if (handle != VK_NULL_HANDLE) {
    vkDestroyDescriptorPool(device, handle, nullptr);
    handle = VK_NULL_HANDLE;
  }
}

}  // namespace tactile
