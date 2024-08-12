// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_fence.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanFence::VulkanFence(VkDevice device, VkFence fence)
  : mDevice {device},
    mFence {fence}
{}

VulkanFence::VulkanFence(VulkanFence&& other) noexcept
  : mDevice {std::exchange(other.mDevice, VK_NULL_HANDLE)},
    mFence {std::exchange(other.mFence, VK_NULL_HANDLE)}
{}

VulkanFence::~VulkanFence() noexcept
{
  _destroy();
}

void VulkanFence::_destroy() noexcept
{
  if (mFence != VK_NULL_HANDLE) {
    vkDestroyFence(mDevice, mFence, nullptr);
    mFence = VK_NULL_HANDLE;
  }
}

auto VulkanFence::operator=(VulkanFence&& other) noexcept -> VulkanFence&
{
  if (this != &other) {
    _destroy();

    mDevice = std::exchange(other.mDevice, VK_NULL_HANDLE);
    mFence = std::exchange(other.mFence, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanFence::create(VkDevice device,
                         const VkFenceCreateFlags flags) -> Expected<VulkanFence, VkResult>
{
  const VkFenceCreateInfo create_info {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = nullptr,
    .flags = flags,
  };

  VkFence fence {};
  const auto result = vkCreateFence(device, &create_info, nullptr, &fence);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan fence: {}", to_string(result));
    return unexpected(result);
  }

  return VulkanFence {device, fence};
}

}  // namespace tactile
