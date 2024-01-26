// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <system_error>  // error_category, error_code
#include <utility>       // to_underlying

#include "tactile/vulkan-renderer/api.hpp"

namespace tactile::vulkan {

/**
 * Provides Vulkan related error codes.
 *
 * \see VkResult
 */
enum class VulkanError : int {  // NOLINT(*-enum-size)
  kUnknown,
  kNotReady,
  kTimeout,
  kIncomplete,
  kOutOfHostMemory,
  kOutOfDeviceMemory,
  kInitializationFailed,
  kDeviceLost,
  kMemoryMapFailed,
  kLayerNotPresent,
  kExtensionNotPresent
};

/**
 * Translates a `VkResult` error code to a `VulkanError` error code.
 *
 * \param vulkan_result A `VkResult` error code.
 *
 * \return
 *    A Vulkan error code.
 */
[[nodiscard]]
TACTILE_VULKAN_API auto translate_vulkan_result(int vulkan_result) -> VulkanError;

/**
 * Returns the error category associated with the Vulkan API.
 *
 * \return
 *    The Vulkan error category.
 */
[[nodiscard]]
TACTILE_VULKAN_API auto get_vulkan_error_category() noexcept
    -> const std::error_category&;

/**
 * Creates a Vulkan error code.
 *
 * \param error The error code.
 *
 * \return
 *    A Vulkan error code.
 */
[[nodiscard]]
inline auto make_error(const VulkanError error) noexcept -> std::error_code
{
  return std::error_code {std::to_underlying(error), get_vulkan_error_category()};
}

[[nodiscard]]
inline auto make_vulkan_error(const VulkanError error) noexcept -> std::error_code
{
  return make_error(error);
}

}  // namespace tactile::vulkan
