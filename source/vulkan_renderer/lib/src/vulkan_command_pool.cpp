// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_command_pool.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanCommandPool::VulkanCommandPool(VkDevice device, VkCommandPool pool)
    : m_device {device},
      m_pool {pool}
{}

VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& other) noexcept
    : m_device {std::exchange(other.m_device, VK_NULL_HANDLE)},
      m_pool {std::exchange(other.m_pool, VK_NULL_HANDLE)}
{}

VulkanCommandPool::~VulkanCommandPool() noexcept
{
  _destroy();
}

auto VulkanCommandPool::operator=(VulkanCommandPool&& other) noexcept -> VulkanCommandPool&
{
  if (this != &other) {
    _destroy();

    m_device = std::exchange(other.m_device, VK_NULL_HANDLE);
    m_pool = std::exchange(other.m_pool, VK_NULL_HANDLE);
  }

  return *this;
}

void VulkanCommandPool::_destroy() noexcept
{
  if (m_pool != VK_NULL_HANDLE) {
    vkDestroyCommandPool(m_device, m_pool, nullptr);
    m_pool = VK_NULL_HANDLE;
  }
}

auto VulkanCommandPool::create(VkDevice device,
                               const std::uint32_t queue_family_index,
                               const VkCommandPoolCreateFlags flags)
    -> std::expected<VulkanCommandPool, VkResult>
{
  const VkCommandPoolCreateInfo pool_info {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = flags,
    .queueFamilyIndex = queue_family_index,
  };

  VkCommandPool pool {};
  const auto result = vkCreateCommandPool(device, &pool_info, nullptr, &pool);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan command pool: {}", to_string(result));
    return std::unexpected {result};
  }

  return VulkanCommandPool {device, pool};
}

auto VulkanCommandPool::device() -> VkDevice
{
  return m_device;
}

auto VulkanCommandPool::get() -> VkCommandPool
{
  return m_pool;
}

}  // namespace tactile
