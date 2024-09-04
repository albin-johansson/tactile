// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <memory>        // unique_ptr
#include <system_error>  // error_code

#include "tactile/base/prelude.hpp"
#include "tactile/base/render/renderer.hpp"
#include "tactile/opengl_renderer/api.hpp"

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
   * \param window The associated window.
   *
   * \return
   * An OpenGL renderer if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto make(IWindow* window) -> std::expected<OpenGLRenderer, std::error_code>;

  OpenGLRenderer(OpenGLRenderer&& other) noexcept;

  auto operator=(OpenGLRenderer&& other) noexcept -> OpenGLRenderer&;

  ~OpenGLRenderer() noexcept override;

  [[nodiscard]]
  auto begin_frame() -> bool override;

  void end_frame() override;

  [[nodiscard]]
  auto load_texture(const std::filesystem::path& image_path)
      -> std::expected<TextureID, std::error_code> override;

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

  [[nodiscard]]
  auto get_imgui_context() -> ImGuiContext* override;

  [[nodiscard]]
  auto imgui_malloc(std::size_t bytes) -> void* override;

  void imgui_free(void* memory) override;

  void process_event(const SDL_Event& event) override;

 private:
  struct Data;
  std::unique_ptr<Data> mData;

  OpenGLRenderer();
};

}  // namespace tactile
