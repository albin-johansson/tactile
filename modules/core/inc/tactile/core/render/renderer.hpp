// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/functional/result.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/render/window.hpp"

TACTILE_FWD(struct ImGuiContext)

namespace tactile {

using ImGuiAllocFn = void*(usize size, void* user_data);
using ImGuiFreeFn = void(void* ptr, void* user_data);

/**
 * \brief Contains memory allocation functions pointers used by Dear ImGui contexts.
 */
struct TACTILE_CORE_API ImGuiAllocatorFunctions final {
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
class TACTILE_CORE_API IRenderer {
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