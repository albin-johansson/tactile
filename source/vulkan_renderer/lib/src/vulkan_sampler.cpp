// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_sampler.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanSampler::VulkanSampler(VkDevice device, VkSampler sampler) noexcept
  : mDevice {device},
    mSampler {sampler}
{}

VulkanSampler::VulkanSampler(VulkanSampler&& other) noexcept
  : mDevice {std::exchange(other.mDevice, VK_NULL_HANDLE)},
    mSampler {std::exchange(other.mSampler, VK_NULL_HANDLE)}
{}

VulkanSampler::~VulkanSampler() noexcept
{
  _destroy();
}

void VulkanSampler::_destroy() noexcept
{
  if (mSampler != VK_NULL_HANDLE) {
    vkDestroySampler(mDevice, mSampler, nullptr);
    mSampler = VK_NULL_HANDLE;
  }
}

auto VulkanSampler::operator=(VulkanSampler&& other) noexcept -> VulkanSampler&
{
  if (this != &other) {
    _destroy();

    mDevice = std::exchange(other.mDevice, VK_NULL_HANDLE);
    mSampler = std::exchange(other.mSampler, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanSampler::create(VkDevice device, const VkSamplerCreateInfo& create_info)
    -> Expected<VulkanSampler, VkResult>
{
  VkSampler sampler {};
  const auto result = vkCreateSampler(device, &create_info, nullptr, &sampler);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan sampler: {}", to_string(result));
    return unexpected(result);
  }

  return VulkanSampler {device, sampler};
}

}  // namespace tactile
