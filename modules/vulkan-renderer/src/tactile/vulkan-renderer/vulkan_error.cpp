// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan-renderer/vulkan_error.hpp"

#include <vulkan/vulkan.h>

namespace tactile::vulkan {
namespace {

class VulkanErrorCategory final : public std::error_category {
 public:
  [[nodiscard]]
  auto name() const noexcept -> const char* override
  {
    return "vulkan";
  }

  [[nodiscard]]
  auto message(const int value) const -> std::string override
  {
    switch (VulkanError {value}) {
      case VulkanError::kUnknown: return "unknown";
      case VulkanError::kNotReady: return "not ready";
      case VulkanError::kTimeout: return "timeout";
      case VulkanError::kIncomplete: return "incomplete";
      case VulkanError::kOutOfHostMemory: return "out of host memory";
      case VulkanError::kOutOfDeviceMemory: return "out of device memory";
      case VulkanError::kInitializationFailed: return "initialization failed";
      case VulkanError::kDeviceLost: return "lost device";
      case VulkanError::kMemoryMapFailed: return "memory map failed";
      case VulkanError::kLayerNotPresent: return "layer not present";
      case VulkanError::kExtensionNotPresent: return "extension not present";
    }

    return "?";
  }
};

}  // namespace

auto translate_vulkan_result(const int vulkan_result) -> VulkanError
{
  switch (static_cast<VkResult>(vulkan_result)) {
    case VK_NOT_READY: return VulkanError::kNotReady;
    case VK_TIMEOUT: return VulkanError::kTimeout;
    case VK_INCOMPLETE: return VulkanError::kIncomplete;
    case VK_ERROR_OUT_OF_HOST_MEMORY: return VulkanError::kOutOfHostMemory;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY: return VulkanError::kOutOfDeviceMemory;
    case VK_ERROR_INITIALIZATION_FAILED: return VulkanError::kInitializationFailed;
    case VK_ERROR_DEVICE_LOST: return VulkanError::kDeviceLost;
    case VK_ERROR_MEMORY_MAP_FAILED: return VulkanError::kMemoryMapFailed;
    case VK_ERROR_LAYER_NOT_PRESENT: return VulkanError::kLayerNotPresent;
    case VK_ERROR_EXTENSION_NOT_PRESENT: return VulkanError::kExtensionNotPresent;

    default: return VulkanError::kUnknown;
  }
}

auto get_vulkan_error_category() noexcept -> const std::error_category&
{
  static VulkanErrorCategory vulkan_error_category {};
  return vulkan_error_category;
}

}  // namespace tactile::vulkan
