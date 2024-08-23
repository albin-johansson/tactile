// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_semaphore.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanSemaphore::VulkanSemaphore(VkDevice device, VkSemaphore semaphore) :
  mDevice {device},
  mSemaphore {semaphore}
{}

VulkanSemaphore::VulkanSemaphore(VulkanSemaphore&& other) noexcept :
  mDevice {std::exchange(other.mDevice, VK_NULL_HANDLE)},
  mSemaphore {std::exchange(other.mSemaphore, VK_NULL_HANDLE)}
{}

VulkanSemaphore::~VulkanSemaphore() noexcept
{
  _destroy();
}

void VulkanSemaphore::_destroy() noexcept
{
  if (mSemaphore != VK_NULL_HANDLE) {
    vkDestroySemaphore(mDevice, mSemaphore, nullptr);
    mSemaphore = VK_NULL_HANDLE;
  }
}

auto VulkanSemaphore::operator=(VulkanSemaphore&& other) noexcept -> VulkanSemaphore&
{
  if (this != &other) {
    _destroy();

    mDevice = std::exchange(other.mDevice, VK_NULL_HANDLE);
    mSemaphore = std::exchange(other.mSemaphore, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanSemaphore::create(VkDevice device) -> std::expected<VulkanSemaphore, VkResult>
{
  const VkSemaphoreCreateInfo create_info {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
  };

  VkSemaphore semaphore {};
  const auto result = vkCreateSemaphore(device, &create_info, nullptr, &semaphore);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan semaphore: {}", to_string(result));
    return std::unexpected {result};
  }

  return VulkanSemaphore {device, semaphore};
}

}  // namespace tactile
