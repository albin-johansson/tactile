/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "vk_gpu.hpp"

#include <algorithm>  // max_element

#include "backend/vk/vk_common.hpp"
#include "backend/vk/vk_context.hpp"
#include "common/debug/assert.hpp"
#include "common/debug/panic.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/set.hpp"

namespace tactile::vk {
namespace {

[[nodiscard]] auto _rate_gpu(GPU gpu) -> int
{
  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(gpu, &properties);

  VkPhysicalDeviceFeatures features;
  vkGetPhysicalDeviceFeatures(gpu, &features);

  int score = 0;

  // Favor discrete (external) graphics cards
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 100;
  }

  return score;
}

[[nodiscard]] auto _has_required_extensions(GPU gpu) -> bool
{
  Set<String> missing_extensions {std::begin(kRequiredDeviceExtensions),
                                  std::end(kRequiredDeviceExtensions)};

  const auto available_extensions = get_extensions(gpu);
  for (const auto& extension: available_extensions) {
    missing_extensions.erase(extension.extensionName);
  }

  return missing_extensions.empty();
}

[[nodiscard]] auto _is_gpu_suitable(GPU gpu, VkSurfaceKHR surface) -> bool
{
  const auto has_extensions = _has_required_extensions(gpu);
  if (!has_extensions) {
    return false;
  }

  const auto queue_family_indices = get_queue_family_indices(gpu, surface);
  const auto has_queue_families = queue_family_indices.present_family.has_value() &&
                                  queue_family_indices.graphics_family.has_value();

  if (!has_queue_families) {
    return false;
  }

  VkPhysicalDeviceFeatures features {};
  vkGetPhysicalDeviceFeatures(gpu, &features);

  if (!features.samplerAnisotropy) {
    return false;
  }

  const auto swap_chain_support = get_swapchain_support(gpu, surface);
  return !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
}

}  // namespace

auto get_available_gpus() -> Vector<GPU>
{
  VkInstance instance = get_global_instance();
  TACTILE_ASSERT(instance != nullptr);

  uint32 gpu_count = 0;
  vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr);

  Vector<GPU> gpus;
  gpus.resize(gpu_count);

  vkEnumeratePhysicalDevices(instance, &gpu_count, gpus.data());

  return gpus;
}

auto get_extensions(GPU gpu) -> Vector<VkExtensionProperties>
{
  uint32 extension_count = 0;
  vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, nullptr);

  Vector<VkExtensionProperties> extensions;
  extensions.resize(extension_count);

  vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extension_count, extensions.data());

  return extensions;
}

auto get_queue_families(GPU gpu) -> Vector<VkQueueFamilyProperties>
{
  uint32 family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);

  Vector<VkQueueFamilyProperties> families;
  families.resize(family_count);

  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

  return families;
}

auto get_queue_family_indices(GPU gpu, VkSurfaceKHR surface) -> QueueFamilyIndices
{
  QueueFamilyIndices indices;

  const auto queue_families = get_queue_families(gpu);

  uint32 index = 0;
  for (const auto& queue_family: queue_families) {
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family = index;
    }

    VkBool32 has_present_support = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, index, surface, &has_present_support);

    if (has_present_support) {
      indices.present_family = index;
    }

    ++index;
  }

  return indices;
}

auto get_surface_formats(GPU gpu, VkSurfaceKHR surface) -> Vector<VkSurfaceFormatKHR>
{
  uint32 format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr);

  Vector<VkSurfaceFormatKHR> formats;
  formats.resize(format_count);

  vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, formats.data());

  return formats;
}

auto get_present_modes(GPU gpu, VkSurfaceKHR surface) -> Vector<VkPresentModeKHR>
{
  uint32 present_mode_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, nullptr);

  Vector<VkPresentModeKHR> present_modes;
  present_modes.resize(present_mode_count);

  vkGetPhysicalDeviceSurfacePresentModesKHR(gpu,
                                            surface,
                                            &present_mode_count,
                                            present_modes.data());

  return present_modes;
}

auto get_swapchain_support(GPU gpu, VkSurfaceKHR surface) -> SwapchainSupport
{
  SwapchainSupport support;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &support.capabilities);

  support.formats = get_surface_formats(gpu, surface);
  support.present_modes = get_present_modes(gpu, surface);

  return support;
}

auto get_suitable_gpu(VkSurfaceKHR surface) -> GPU
{
  TACTILE_ASSERT(get_global_instance() != nullptr);

  const auto gpus = get_available_gpus();

  if (gpus.empty()) {
    throw TactileError {"No available GPUs with Vulkan support"};
  }

  // Rate available GPUs and select the best one
  HashMap<GPU, int> gpu_scores;
  gpu_scores.reserve(gpus.size());

  for (const auto& gpu: gpus) {
    gpu_scores[gpu] = _is_gpu_suitable(gpu, surface) ? _rate_gpu(gpu) : -1;
  }

  const auto gpu_iter = std::max_element(gpu_scores.begin(), gpu_scores.end());
  auto* best_gpu = gpu_iter->first;

  if (best_gpu == VK_NULL_HANDLE) {
    throw TactileError {"Found no suitable GPU"};
  }

  return best_gpu;
}

}  // namespace tactile::vk
