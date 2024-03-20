// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/opengl/api.hpp"

struct ImDrawData;

namespace tactile {

/**
 * RAII type that manages an ImGui OpenGL backendL.
 */
class TACTILE_OPENGL_API OpenGLImGuiImplOpenGL3 final {
 public:
  /**
   * Initializes the ImGui OpenGL 3 renderer.
   *
   * \return
   *    An RAII wrapper if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto init() -> Result<OpenGLImGuiImplOpenGL3>;

  /** Shuts down the renderer. */
  ~OpenGLImGuiImplOpenGL3() noexcept;

  OpenGLImGuiImplOpenGL3(OpenGLImGuiImplOpenGL3&& other) noexcept;

  auto operator=(OpenGLImGuiImplOpenGL3&&) noexcept -> OpenGLImGuiImplOpenGL3& = delete;

  TACTILE_DELETE_COPY(OpenGLImGuiImplOpenGL3);

  /** Marks the beginning of a new frame. */
  void begin_frame();

  /**
   * Submits draw data to be rendered.
   *
   * \param draw_data The data that will be rendered.
   */
  void render(ImDrawData* draw_data);

  /** Destroys and restores the font atlas texture. */
  void reload_fonts_texture();

 private:
  bool mActive {true};

  OpenGLImGuiImplOpenGL3() noexcept = default;
};

}  // namespace tactile
