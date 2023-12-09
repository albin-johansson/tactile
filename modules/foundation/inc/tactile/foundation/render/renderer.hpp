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

using ImGuiAllocFn = void*(usize size, void* user_data);
using ImGuiFreeFn = void(void* ptr, void* user_data);

/**
 * \brief Contains memory allocation functions pointers used by Dear ImGui contexts.
 */
struct ImGuiAllocatorFunctions final {
  ImGuiAllocFn* alloc_fn;
  ImGuiFreeFn* free_fn;
};

/**
 * \interface IRenderer
 * \brief Provides the high level renderer backend API.
 *
 * \details Every renderer backend plugin needs to provide an implementation of this
 *          interface, and register an instance of it using the `RenderContext` class.
 */
class IRenderer {
 public:
  TACTILE_INTERFACE_CLASS(IRenderer);

  /**
   * \brief Attempts to begin a new render frame.
   *
   * \return nothing if successful, or an error code if something went wrong.
   */
  [[nodiscard]]
  virtual auto begin_frame() -> Result<void> = 0;

  /**
   * \brief Ends the current render frame.
   *
   * \pre The `begin_frame` function must be successfully called before this function.
   */
  virtual void end_frame() = 0;

  /**
   * \brief Reloads the fonts texture, if possible.
   */
  virtual void reload_fonts_texture() = 0;

  /**
   * \brief Indicates whether the renderer supports reloading the fonts texture.
   *
   * \return true if the fonts texture can be reloaded; false otherwise.
   */
  [[nodiscard]]
  virtual auto can_reload_fonts_texture() const -> bool = 0;

  /**
   * \brief Tries to load a texture from disk.
   *
   * \param image_path the path to the image.
   *
   * \return a non-owning pointer to the loaded texture (may be null).
   */
  [[nodiscard]]
  virtual auto load_texture(const FilePath& image_path) -> ITexture* = 0;

  /**
   * \brief Returns the associated window.
   *
   * \return a window.
   */
  [[nodiscard]]
  virtual auto get_window() -> IWindow* = 0;

  /**
   * \copydoc get_window
   */
  [[nodiscard]]
  virtual auto get_window() const -> const IWindow* = 0;

  /**
   * \brief Returns the associated Dear ImGui context.
   *
   * \details Users of the renderer must explicitly install the associated Dear ImGui
   *          context and memory management functions before issuing other Dear ImGui
   *          commands. Use `ImGui::SetCurrentContext` and `ImGui::SetAllocatorFunctions`
   *          to accomplish this.
   *
   * \return a Dear ImGui context.
   */
  [[nodiscard]]
  virtual auto get_imgui_context() -> ImGuiContext* = 0;

  /**
   * \brief Returns the memory management functions used by the associated ImGui context.
   *
   * \return the Dear ImGui memory management function pointers.
   */
  [[nodiscard]]
  virtual auto get_imgui_allocator_functions() const -> ImGuiAllocatorFunctions = 0;
};

}  // namespace tactile