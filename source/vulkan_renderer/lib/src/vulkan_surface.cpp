// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_surface.hpp"

#include <utility>  // exchange

#include <SDL2/SDL_vulkan.h>

#include "tactile/base/render/window.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

VulkanSurface::VulkanSurface(VkInstance instance, VkSurfaceKHR surface) :
  mInstance {instance},
  mSurface {surface}
{}

VulkanSurface::VulkanSurface(VulkanSurface&& other) noexcept :
  mInstance {std::exchange(other.mInstance, VK_NULL_HANDLE)},
  mSurface {std::exchange(other.mSurface, VK_NULL_HANDLE)}
{}

VulkanSurface::~VulkanSurface() noexcept
{
  _destroy();
}

void VulkanSurface::_destroy() noexcept
{
  if (mSurface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    mSurface = VK_NULL_HANDLE;
  }
}

auto VulkanSurface::operator=(VulkanSurface&& other) noexcept -> VulkanSurface&
{
  if (this != &other) {
    _destroy();

    mInstance = std::exchange(other.mInstance, VK_NULL_HANDLE);
    mSurface = std::exchange(other.mSurface, VK_NULL_HANDLE);
  }

  return *this;
}

auto VulkanSurface::create(VkInstance instance,
                           IWindow* window) -> std::expected<VulkanSurface, VkResult>
{
  VkSurfaceKHR surface {};

  if (SDL_Vulkan_CreateSurface(window->get_handle(), instance, &surface) == SDL_FALSE) {
    log(LogLevel::kError, "Could not create Vulkan surface: {}", SDL_GetError());
    return std::unexpected {VK_ERROR_UNKNOWN};
  }

  return VulkanSurface {instance, surface};
}

}  // namespace tactile
