// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vulkan/vulkan.h>

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/window.hpp"
#include "tactile/vulkan-renderer/api.hpp"

struct SDL_Window;

namespace tactile::vulkan {

/**
 * A deleter for Vulkan backed SDL windows.
 */
struct TACTILE_VULKAN_API SDLVulkanWindowDeleter final {
  void operator()(SDL_Window* window) noexcept;
};

/**
 * A Vulkan 1.2 window implementation.
 */
class TACTILE_VULKAN_API VulkanWindow final : public IWindow {
 public:
  TACTILE_DELETE_COPY(VulkanWindow);
  TACTILE_DEFAULT_MOVE(VulkanWindow);

  /**
   * Creates a Vulkan window.
   *
   * \return
   *    A Vulkan window, or an error code.
   */
  [[nodiscard]]
  static auto create() -> Result<VulkanWindow>;

  ~VulkanWindow() noexcept override = default;

  void show() override;

  void hide() override;

  void maximize() override;

  /**
   * Returns the underlying window.
   *
   * \return
   *    A window handle.
   */
  [[nodiscard]]
  auto get_handle() -> SDL_Window*;

 private:
  Unique<SDL_Window, SDLVulkanWindowDeleter> mWindow;

  explicit VulkanWindow(SDL_Window* window);
};

}  // namespace tactile::vulkan
