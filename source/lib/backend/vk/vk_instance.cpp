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

#include "vk_instance.hpp"

#include <SDL2/SDL_vulkan.h>
#include <spdlog/spdlog.h>

#include "backend/vk/vk_common.hpp"
#include "backend/vk/vk_context.hpp"
#include "backend/vk/vk_error.hpp"
#include "common/predef.hpp"
#include "common/primitives.hpp"
#include "common/type/vector.hpp"

namespace tactile::vk {
namespace {

[[nodiscard]] auto _get_instance_extension_names(SDL_Window* window)
    -> Vector<const char*>
{
  uint32 extension_count = 0;
  SDL_Vulkan_GetInstanceExtensions(window, &extension_count, nullptr);

  Vector<const char*> extensions;
  extensions.resize(extension_count);
  SDL_Vulkan_GetInstanceExtensions(window, &extension_count, extensions.data());

#ifdef TACTILE_USE_VULKAN_SUBSET
  extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif  // TACTILE_USE_VULKAN_SUBSET

  if constexpr (kIsDebugBuild) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}
}  // namespace

void InstanceDeleter::operator()(VkInstance instance) noexcept
{
  vkDestroyInstance(instance, nullptr);
}

auto create_instance(SDL_Window* window) -> UniqueInstance
{
  spdlog::trace("[Vulkan] Creating instance");

  const auto instance_extensions = _get_instance_extension_names(window);

  const VkApplicationInfo app_info {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = nullptr,
      .pApplicationName = "Tactile",
      .applicationVersion = VK_MAKE_VERSION(0, 6, 0),
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(0, 1, 0),
      .apiVersion = kVulkanApiVersion,
  };

  VkInstanceCreateInfo instance_info {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .pApplicationInfo = &app_info,
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = nullptr,
      .enabledExtensionCount = static_cast<uint32>(instance_extensions.size()),
      .ppEnabledExtensionNames = instance_extensions.data(),
  };

#ifdef TACTILE_USE_VULKAN_SUBSET
  // Allow implementations that only provide a subset of the Vulkan spec, e.g. MoltenVK.
  instance_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif  // TACTILE_USE_VULKAN_SUBSET

  if constexpr (kIsDebugBuild) {
    spdlog::debug("[Vulkan] Enabling validation layers");

    // Enable validation layers in debug builds
    instance_info.ppEnabledLayerNames = kValidationLayerNames;
    instance_info.enabledLayerCount = std::size(kValidationLayerNames);
  }

  VkInstance instance = VK_NULL_HANDLE;
  if (const auto res = vkCreateInstance(&instance_info, nullptr, &instance);
      res != VK_SUCCESS) {
    throw VulkanError {"Could not create Vulkan instance", res};
  }

  set_global_instance(instance);
  return UniqueInstance {instance};
}

}  // namespace tactile::vk
