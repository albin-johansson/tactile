// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/opengl/api.hpp"

struct ImGuiContext;

namespace tactile {

/**
 * RAII type that manages an ImGui context using OpenGL.
 */
class TACTILE_OPENGL_API OpenGLImGuiContext final {
 public:
  /**
   * Creates a context.
   *
   * \return
   *    An OpenGL ImGui context; or an error code if something went wrong.
   */
  [[nodiscard]]
  static auto make() -> Result<OpenGLImGuiContext>;

  /**
   * Destroys the associated context.
   */
  ~OpenGLImGuiContext() noexcept;

  OpenGLImGuiContext(OpenGLImGuiContext&& other) noexcept;

  auto operator=(OpenGLImGuiContext&& other) noexcept -> OpenGLImGuiContext&;

  TACTILE_DELETE_COPY(OpenGLImGuiContext);

  [[nodiscard]]
  auto get() noexcept -> ImGuiContext*;

 private:
  ImGuiContext* mContext;

  explicit OpenGLImGuiContext(ImGuiContext* context) noexcept;

  void _dispose() noexcept;
};

}  // namespace tactile
