// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

struct ImGuiContext;

namespace tactile {

class IWindow;
class ITexture;

/**
 * Provides the high level renderer backend API.
 */
class IRenderer {
 public:
  TACTILE_INTERFACE_CLASS(IRenderer);

  /**
   * Attempts to begin a new render frame.
   *
   * \return
   *    True if successful; false if something went wrong.
   */
  [[nodiscard]]
  virtual auto begin_frame() -> bool = 0;

  /**
   * Ends the current render frame.
   *
   * \pre The \c begin_frame function must be successfully called before this function.
   */
  virtual void end_frame() = 0;

  /**
   * Tries to load a texture from disk.
   *
   * \param image_path The path to the image.
   *
   * \return
   *    A non-owning pointer to the loaded texture (may be null).
   */
  [[nodiscard]]
  virtual auto load_texture(const char* image_path) -> ITexture* = 0;

  /**
   * Reloads the fonts texture, if possible.
   */
  virtual void try_reload_fonts() = 0;

  /**
   * Indicates whether the renderer supports reloading the fonts texture.
   *
   * \return
   *    True if the fonts texture can be reloaded; false otherwise.
   */
  [[nodiscard]]
  virtual auto can_reload_fonts() const -> bool = 0;

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
   *    commands. Use \c ImGui::SetCurrentContext and \c ImGui::SetAllocatorFunctions
   *    to accomplish this.
   *
   * \return
   *    A Dear ImGui context.
   */
  [[nodiscard]]
  virtual auto get_imgui_context() -> ImGuiContext* = 0;
};

}  // namespace tactile
