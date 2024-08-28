// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_sampler.hpp"

#include <utility>  // exchange

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanSampler::VulkanSampler(VkDevice device, VkSampler sampler) noexcept
    : m_device {device},
      m_sampler {sampler}
{}

VulkanSampler::VulkanSampler(VulkanSampler&& other) noexcept
    : m_device {std::exchange(other.m_device, VK_NULL_HANDLE)},
      m_sampler {std::exchange(other.m_sampler, VK_NULL_HANDLE)}
{}

VulkanSampler::~VulkanSampler() noexcept
{
  _destroy();
}

void VulkanSampler::_destroy() noexcept
{
  if (m_sampler != VK_NULL_HANDLE) {
    vkDestroySampler(m_device, m_sampler, nullptr);
    m_sampler = VK_NULL_HANDLE;
  }
}

auto VulkanSampler::operator=(VulkanSampler&& other) noexcept -> VulkanSampler&
{
  if (this != &other) {
    _destroy();

    m_device = std::exchange(other.m_device, VK_NULL_HANDLE);
    m_sampler = std::exchange(other.m_sampler, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanSampler::create(VkDevice device) -> std::expected<VulkanSampler, VkResult>
{
  constexpr VkSamplerCreateInfo create_info {
    .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .magFilter = VK_FILTER_NEAREST,
    .minFilter = VK_FILTER_NEAREST,
    .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
    .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .mipLodBias = 0.0f,
    .anisotropyEnable = VK_FALSE,
    .maxAnisotropy = 0.0f,
    .compareEnable = VK_FALSE,
    .compareOp = VK_COMPARE_OP_NEVER,
    .minLod = 0.0f,
    .maxLod = 64.0f,
    .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    .unnormalizedCoordinates = VK_FALSE,
  };

  VkSampler sampler {};
  const auto result = vkCreateSampler(device, &create_info, nullptr, &sampler);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan sampler: {}", to_string(result));
    return std::unexpected {result};
  }

  return VulkanSampler {device, sampler};
}

auto VulkanSampler::device() -> VkDevice
{
  return m_device;
}

auto VulkanSampler::get() -> VkSampler
{
  return m_sampler;
}

}  // namespace tactile
