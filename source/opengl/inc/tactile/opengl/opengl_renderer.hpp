// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/opengl/api.hpp"
#include "tactile/render/renderer.hpp"

namespace tactile {

/**
 * Represents an OpenGL 4.1.0 renderer.
 */
class TACTILE_OPENGL_API OpenGLRenderer final : public IRenderer
{
 public:
  TACTILE_DELETE_COPY(OpenGLRenderer);

  /**
   * Creates an OpenGL renderer, along with an associated window.
   *
   * \return
   *    An OpenGL renderer if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto make() -> Result<OpenGLRenderer>;

  OpenGLRenderer(OpenGLRenderer&& other) noexcept;

  auto operator=(OpenGLRenderer&& other) noexcept -> OpenGLRenderer&;

  ~OpenGLRenderer() noexcept final;

  [[nodiscard]]
  auto begin_frame() -> bool override;

  void end_frame() override;

  [[nodiscard]]
  auto load_texture(const char* image_path) -> ITexture* override;

  void try_reload_fonts() override;

  [[nodiscard]]
  auto can_reload_fonts() const -> bool override;

  [[nodiscard]]
  auto get_window() -> IWindow* override;

  [[nodiscard]]
  auto get_window() const -> const IWindow* override;

  [[nodiscard]]
  auto get_imgui_context() -> ImGuiContext* override;

 private:
  struct Data;
  Unique<Data> mData;

  OpenGLRenderer();
};

extern "C"
{
  TACTILE_OPENGL_API auto tactile_make_renderer() -> IRenderer*;

  TACTILE_OPENGL_API void tactile_free_renderer(IRenderer* renderer);
}

}  // namespace tactile
