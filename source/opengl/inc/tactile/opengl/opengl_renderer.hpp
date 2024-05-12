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
   * Creates an OpenGL renderer.
   *
   * \param window  The associated window.
   * \param context The associated ImGui context.
   *
   * \return
   * An OpenGL renderer if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto make(IWindow* window,
                   ImGuiContext* context) -> Result<OpenGLRenderer>;

  OpenGLRenderer(OpenGLRenderer&& other) noexcept;

  auto operator=(OpenGLRenderer&& other) noexcept -> OpenGLRenderer&;

  ~OpenGLRenderer() noexcept final;

  [[nodiscard]]
  auto begin_frame() -> bool override;

  void end_frame() override;

  [[nodiscard]]
  auto load_texture(const char* image_path) -> Result<TextureID> override;

  void unload_texture(TextureID id) override;

  [[nodiscard]]
  auto find_texture(TextureID id) const -> const ITexture* override;

  void try_reload_fonts() override;

  [[nodiscard]]
  auto can_reload_fonts() const -> bool override;

  [[nodiscard]]
  auto get_window() -> IWindow* override;

  [[nodiscard]]
  auto get_window() const -> const IWindow* override;

 private:
  struct Data;
  Unique<Data> mData;

  OpenGLRenderer();
};

extern "C"
{
  TACTILE_OPENGL_API
  void tactile_prepare_renderer(uint32* window_flags);

  TACTILE_OPENGL_API
  auto tactile_make_renderer(IWindow* window,
                             ImGuiContext* context) -> IRenderer*;

  TACTILE_OPENGL_API
  void tactile_free_renderer(IRenderer* renderer);
}

}  // namespace tactile
