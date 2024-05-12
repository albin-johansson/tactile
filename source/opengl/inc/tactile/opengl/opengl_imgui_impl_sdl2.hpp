// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/opengl/api.hpp"

struct ImGuiContext;

namespace tactile {

class IWindow;

/**
 * RAII type that manages an ImGui SDL2 backend using OpenGL.
 */
class TACTILE_OPENGL_API OpenGLImGuiImplSDL2 final
{
 public:
  TACTILE_DELETE_COPY(OpenGLImGuiImplSDL2);

  /**
   * Initializes the ImGui SDL2 backend.
   *
   * \param window  The associated window.
   * \param context The associated Dear ImGui context.
   *
   * \return
   * An RAII wrapper if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto init(IWindow& window,
                   ImGuiContext* context) -> Result<OpenGLImGuiImplSDL2>;

  /**
   * Shuts down the backend.
   */
  ~OpenGLImGuiImplSDL2() noexcept;

  OpenGLImGuiImplSDL2(OpenGLImGuiImplSDL2&& other) noexcept;

  auto operator=(OpenGLImGuiImplSDL2&&) noexcept -> OpenGLImGuiImplSDL2& =
                                                        delete;

  /**
   * Marks the beginning of a new frame.
   */
  void begin_frame();

 private:
  bool mActive {true};

  OpenGLImGuiImplSDL2() noexcept = default;
};

}  // namespace tactile
