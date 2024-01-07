// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan-renderer/vulkan_window.hpp"

#include <SDL2/SDL.h>

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::vulkan {

void SDLVulkanWindowDeleter::operator()(SDL_Window* window) noexcept
{
  SDL_DestroyWindow(window);
}

auto VulkanWindow::create() -> Result<VulkanWindow>
{
  auto* window =
      SDL_CreateWindow("Tactile " TACTILE_VERSION_STRING,
                       SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED,
                       800,
                       600,
                       SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN);

  if (!window) {
    TACTILE_LOG_ERROR("[Vulkan] Could not create window: {}", SDL_GetError());
    return unexpected(make_generic_error(GenericError::kInitFailure));
  }

  return VulkanWindow {window};
}

VulkanWindow::VulkanWindow(SDL_Window* window)
  : mWindow {require_not_null(window, "null window")}
{}

void VulkanWindow::show()
{
  SDL_ShowWindow(mWindow.get());
}

void VulkanWindow::hide()
{
  SDL_HideWindow(mWindow.get());
}

void VulkanWindow::maximize()
{
  SDL_MaximizeWindow(mWindow.get());
}

auto VulkanWindow::get_handle() -> SDL_Window*
{
  return mWindow.get();
}

}  // namespace tactile::vulkan
