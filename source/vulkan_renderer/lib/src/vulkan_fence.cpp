// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_fence.hpp"

#include <cstdint>  // uint64_t
#include <limits>   // numeric_limits
#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanFence::VulkanFence(VkDevice device, VkFence fence)
    : m_device {device},
      m_fence {fence}
{}

VulkanFence::VulkanFence(VulkanFence&& other) noexcept
    : m_device {std::exchange(other.m_device, VK_NULL_HANDLE)},
      m_fence {std::exchange(other.m_fence, VK_NULL_HANDLE)}
{}

VulkanFence::~VulkanFence() noexcept
{
  _destroy();
}

void VulkanFence::_destroy() noexcept
{
  if (m_fence != VK_NULL_HANDLE) {
    vkDestroyFence(m_device, m_fence, nullptr);
    m_fence = VK_NULL_HANDLE;
  }
}

auto VulkanFence::operator=(VulkanFence&& other) noexcept -> VulkanFence&
{
  if (this != &other) {
    _destroy();

    m_device = std::exchange(other.m_device, VK_NULL_HANDLE);
    m_fence = std::exchange(other.m_fence, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanFence::create(VkDevice device, const VkFenceCreateFlags flags)
    -> std::expected<VulkanFence, VkResult>
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
    return std::unexpected {result};
  }

  return VulkanFence {device, fence};
}

auto VulkanFence::reset() -> VkResult
{
  return vkResetFences(m_device, 1, &m_fence);
}

auto VulkanFence::wait() -> VkResult
{
  return vkWaitForFences(m_device,
                         1,
                         &m_fence,
                         VK_TRUE,
                         std::numeric_limits<std::uint64_t>::max());
}

auto VulkanFence::device() -> VkDevice
{
  return m_device;
}

auto VulkanFence::get() -> VkFence
{
  return m_fence;
}

}  // namespace tactile
