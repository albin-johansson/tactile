// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_device.hpp"

#include <set>      // set
#include <utility>  // exchange
#include <vector>   // vector

#include "tactile/runtime/logging.hpp"
#include "tactile/vulkan_renderer/vulkan_physical_device.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

VulkanDevice::VulkanDevice(VkDevice device,
                           const std::uint32_t graphics_queue_index,
                           const std::uint32_t present_queue_index)
    : m_device {device},
      m_graphics_queue_index {graphics_queue_index},
      m_present_queue_index {present_queue_index}
{}

VulkanDevice::VulkanDevice(VulkanDevice&& other) noexcept
    : m_device {std::exchange(other.m_device, VK_NULL_HANDLE)},
      m_graphics_queue_index {std::exchange(other.m_graphics_queue_index, 0)},
      m_present_queue_index {std::exchange(other.m_present_queue_index, 0)}
{}

VulkanDevice::~VulkanDevice() noexcept
{
  _destroy();
}

void VulkanDevice::_destroy() noexcept
{
  if (m_device != VK_NULL_HANDLE) {
    vkDestroyDevice(m_device, nullptr);
    m_device = VK_NULL_HANDLE;
  }
}

auto VulkanDevice::operator=(VulkanDevice&& other) noexcept -> VulkanDevice&
{
  if (this != &other) {
    _destroy();

    m_device = std::exchange(other.m_device, VK_NULL_HANDLE);
    m_graphics_queue_index = std::exchange(other.m_graphics_queue_index, 0);
    m_present_queue_index = std::exchange(other.m_present_queue_index, 0);
  }

  return *this;
}

auto VulkanDevice::create(VkPhysicalDevice physical_device,
                          VkSurfaceKHR surface) -> std::expected<VulkanDevice, VkResult>
{
  const auto queue_family_indices = get_queue_family_indices(physical_device, surface);

  if (!queue_family_indices.graphics.has_value() ||
      !queue_family_indices.present.has_value()) {
    log(LogLevel::kError, "Missing Vulkan graphics or presentation queue");
    return std::unexpected {VK_ERROR_UNKNOWN};
  }

  const std::set unique_queue_family_indices = {
    queue_family_indices.graphics.value(),
    queue_family_indices.present.value(),
  };

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos {};
  queue_create_infos.reserve(unique_queue_family_indices.size());

  constexpr float queue_priority = 1.0f;

  for (const auto queue_family_index : unique_queue_family_indices) {
    queue_create_infos.push_back(VkDeviceQueueCreateInfo {
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .queueFamilyIndex = queue_family_index,
      .queueCount = 1,
      .pQueuePriorities = &queue_priority,
    });
  }

  std::vector<const char*> enabled_layers {};
  if constexpr (kIsDebugBuild) {
    enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  std::vector<const char*> enabled_extensions {};
  enabled_extensions.reserve(3);
  enabled_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  enabled_extensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);

#ifdef TACTILE_USE_VULKAN_SUBSET
  enabled_extensions.push_back("VK_KHR_portability_subset");
#endif  // TACTILE_USE_VULKAN_SUBSET

  for (const auto* extension : enabled_extensions) {
    log(LogLevel::kDebug, "Using Vulkan device extension '{}'", extension);
  }

  constexpr VkPhysicalDeviceFeatures enabled_features {};

  constexpr VkPhysicalDeviceDynamicRenderingFeaturesKHR enabled_dynamic_rendering_features {
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
    .pNext = nullptr,
    .dynamicRendering = VK_TRUE,
  };

  const VkDeviceCreateInfo device_info {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = &enabled_dynamic_rendering_features,
    .flags = 0,
    .queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size()),
    .pQueueCreateInfos = queue_create_infos.data(),
    .enabledLayerCount = static_cast<std::uint32_t>(enabled_layers.size()),
    .ppEnabledLayerNames = enabled_layers.data(),
    .enabledExtensionCount = static_cast<std::uint32_t>(enabled_extensions.size()),
    .ppEnabledExtensionNames = enabled_extensions.data(),
    .pEnabledFeatures = &enabled_features,
  };

  VkDevice device {};
  const auto result = vkCreateDevice(physical_device, &device_info, nullptr, &device);

  if (result != VK_SUCCESS) {
    log(LogLevel::kError, "Could not create Vulkan device: {}", to_string(result));
    return std::unexpected {result};
  }

  return VulkanDevice {device, *queue_family_indices.graphics, *queue_family_indices.present};
}

auto VulkanDevice::get() -> VkDevice
{
  return m_device;
}

auto VulkanDevice::graphics_queue_index() const -> std::uint32_t
{
  return m_graphics_queue_index;
}

auto VulkanDevice::presentation_queue_index() const -> std::uint32_t
{
  return m_present_queue_index;
}

}  // namespace tactile
