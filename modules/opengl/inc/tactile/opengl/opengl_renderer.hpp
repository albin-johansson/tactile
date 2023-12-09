// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/tree_map.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/renderer.hpp"
#include "tactile/opengl/api.hpp"
#include "tactile/opengl/opengl_window.hpp"

namespace tactile::gl {

/**
 * \brief An OpenGL 4 renderer implementation.
 */
class TACTILE_OPENGL_API OpenGLRenderer final : public IRenderer {
 public:
  TACTILE_DELETE_COPY(OpenGLRenderer);
  TACTILE_DELETE_MOVE(OpenGLRenderer);

  /**
   * \brief Creates a renderer and an associated Dear ImGui context.
   *
   * \param window the associated window.
   *
   * \throw Error if the provided window is null.
   * \throw Error if the Dear ImGui resources cannot be initialized.
   */
  explicit OpenGLRenderer(OpenGLWindow* window);

  ~OpenGLRenderer() noexcept override;

  [[nodiscard]]
  auto begin_frame() -> Result<void> override;

  void end_frame() override;

  void reload_fonts_texture() override;

  [[nodiscard]]
  auto can_reload_fonts_texture() const -> bool override;

  [[nodiscard]]
  auto get_window() -> OpenGLWindow* override;

  [[nodiscard]]
  auto get_window() const -> const OpenGLWindow* override;

  [[nodiscard]]
  auto get_imgui_context() -> ImGuiContext* override;

  [[nodiscard]]
  auto get_imgui_allocator_functions() const -> ImGuiAllocatorFunctions override;

 private:
  OpenGLWindow* mWindow;
  ImGuiContext* mImGuiContext;
};

}  // namespace tactile::gl