// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/render/texture.hpp"
#include "tactile/opengl_renderer/api.hpp"

namespace tactile {

struct RendererOptions;

/**
 * Represents an OpenGL texture.
 */
class TACTILE_OPENGL_API OpenGLTexture final : public ITexture
{
 public:
  using id_type = uint;

  /**
   * Loads a texture from an image on disk.
   *
   * \param image_path The path to the image file.
   * \param options    The renderer options to use.
   *
   * \return
   * A texture if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto load(const std::filesystem::path& image_path, const RendererOptions& options)
      -> std::expected<OpenGLTexture, std::error_code>;

  OpenGLTexture() = delete;

  ~OpenGLTexture() noexcept override;

  OpenGLTexture(OpenGLTexture&& other) noexcept;

  auto operator=(OpenGLTexture&& other) noexcept -> OpenGLTexture&;

  TACTILE_DELETE_COPY(OpenGLTexture);

  [[nodiscard]]
  auto get_handle() const -> void* override;

  [[nodiscard]]
  auto get_size() const -> TextureSize override;

  [[nodiscard]]
  auto get_path() const -> const std::filesystem::path& override;

 private:
  id_type mID;
  TextureSize mSize;
  std::filesystem::path mPath;

  OpenGLTexture(id_type id, TextureSize size, std::filesystem::path path);

  void _dispose() noexcept;
};

}  // namespace tactile
