// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

#include "tactile/base/prelude.hpp"
#include "tactile/opengl_renderer/api.hpp"

struct SDL_Window;
struct ImGuiContext;

namespace tactile {

/**
 * RAII helper for an ImGui backend using OpenGL and SDL2.
 */
class TACTILE_OPENGL_API GLImGuiBackendWrapper final
{
 public:
  TACTILE_DELETE_COPY(GLImGuiBackendWrapper);

  /**
   * Initializes the ImGui SDL2 backend.
   *
   * \param window  The associated window.
   * \param context The associated Dear ImGui context.
   *
   * \return
   * An RAII object if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto init(SDL_Window* window, ImGuiContext* context)
      -> std::expected<GLImGuiBackendWrapper, std::error_code>;

  /**
   * Shuts down the backend.
   */
  ~GLImGuiBackendWrapper() noexcept;

  GLImGuiBackendWrapper(GLImGuiBackendWrapper&& other) noexcept;

  auto operator=(GLImGuiBackendWrapper&&) noexcept  //
      -> GLImGuiBackendWrapper& = delete;

 private:
  bool mActive {};

  GLImGuiBackendWrapper() noexcept;
};

/**
 * RAII helper for an ImGui renderer backend using OpenGL.
 */
class TACTILE_OPENGL_API GLImGuiRendererWrapper final
{
 public:
  TACTILE_DELETE_COPY(GLImGuiRendererWrapper);

  /**
   * Initializes the ImGui OpenGL renderer.
   *
   * \return
   * An RAII object if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto init() -> std::expected<GLImGuiRendererWrapper, std::error_code>;

  /**
   * Shuts down the renderer.
   */
  ~GLImGuiRendererWrapper() noexcept;

  GLImGuiRendererWrapper(GLImGuiRendererWrapper&& other) noexcept;

  auto operator=(GLImGuiRendererWrapper&&) noexcept  //
      -> GLImGuiRendererWrapper& = delete;

 private:
  bool mActive {};

  GLImGuiRendererWrapper() noexcept;
};

}  // namespace tactile
