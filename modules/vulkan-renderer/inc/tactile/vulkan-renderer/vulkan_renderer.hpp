// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vulkan/vulkan.h>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/renderer.hpp"
#include "tactile/vulkan-renderer/api.hpp"

namespace tactile::vulkan {

class VulkanWindow;

/**
 * A Vulkan 1.2 renderer implementation.
 */
class TACTILE_VULKAN_API VulkanRenderer final : public IRenderer {
 public:
  TACTILE_DELETE_COPY(VulkanRenderer);

  /**
   * Creates a Vulkan renderer.
   *
   * \param window The associated window.
   *
   * \return
   *    A Vulkan renderer, or an error code.
   */
  [[nodiscard]]
  static auto create(VulkanWindow* window) -> Result<VulkanRenderer>;

  ~VulkanRenderer() noexcept override;

  VulkanRenderer(VulkanRenderer&& other) noexcept;

  auto operator=(VulkanRenderer&& other) noexcept -> VulkanRenderer&;

  [[nodiscard]]
  auto begin_frame() -> Result<void> override;

  void end_frame() override;

  void reload_fonts_texture() override;

  [[nodiscard]]
  auto can_reload_fonts_texture() const -> bool override;

  [[nodiscard]]
  auto load_texture(const FilePath& image_path) -> ITexture* override;

  [[nodiscard]]
  auto get_window() -> IWindow* override;

  [[nodiscard]]
  auto get_window() const -> const IWindow* override;

  [[nodiscard]]
  auto get_imgui_context() -> ImGuiContext* override;

 private:
  VulkanWindow* mWindow;
  ImGuiContext* mImGuiContext;
  bool mPrimed {false};

  VulkanRenderer(VulkanWindow* window, ImGuiContext* imgui_context);

  void _dispose() noexcept;
};

}  // namespace tactile::vulkan
