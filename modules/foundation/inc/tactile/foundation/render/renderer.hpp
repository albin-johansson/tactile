// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/texture.hpp"
#include "tactile/foundation/render/window.hpp"

struct ImGuiContext;

namespace tactile {

/**
 * Provides the high level renderer backend API.
 *
 * \details
 *    Every renderer backend plugin needs to provide an implementation of this
 *    interface, and register an instance of it using the `RenderContext` class.
 */
class IRenderer {
 public:
  TACTILE_INTERFACE_CLASS(IRenderer);

  /**
   * Attempts to begin a new render frame.
   *
   * \return
   *    Nothing if successful, or an error code if something went wrong.
   */
  [[nodiscard]]
  virtual auto begin_frame() -> Result<void> = 0;

  /**
   * Ends the current render frame.
   *
   * \pre The `begin_frame` function must be successfully called before this function.
   */
  virtual void end_frame() = 0;

  /**
   * Reloads the fonts texture, if possible.
   */
  virtual void reload_fonts_texture() = 0;

  /**
   * Indicates whether the renderer supports reloading the fonts texture.
   *
   * \return
   *    True if the fonts texture can be reloaded; false otherwise.
   */
  [[nodiscard]]
  virtual auto can_reload_fonts_texture() const -> bool = 0;

  /**
   * Tries to load a texture from disk.
   *
   * \param image_path The path to the image.
   *
   * \return
   *    A non-owning pointer to the loaded texture (may be null).
   */
  [[nodiscard]]
  virtual auto load_texture(const FilePath& image_path) -> ITexture* = 0;

  /**
   * Returns the associated window.
   *
   * \return
   *    A window.
   */
  [[nodiscard]]
  virtual auto get_window() -> IWindow* = 0;

  /** \copydoc get_window() */
  [[nodiscard]]
  virtual auto get_window() const -> const IWindow* = 0;

  /**
   * Returns the associated Dear ImGui context.
   *
   * \details
   *    Users of the renderer must explicitly install the associated Dear ImGui
   *    context and memory management functions before issuing other Dear ImGui
   *    commands. Use `ImGui::SetCurrentContext` and `ImGui::SetAllocatorFunctions`
   *    to accomplish this.
   *
   * \return
   *    A Dear ImGui context.
   */
  [[nodiscard]]
  virtual auto get_imgui_context() -> ImGuiContext* = 0;
};

}  // namespace tactile