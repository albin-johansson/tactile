// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/render/texture.hpp"
#include "tactile/null_renderer/api.hpp"

namespace tactile {

/**
 * A null texture implementation.
 */
class TACTILE_NULL_RENDERER_API NullTexture final : public ITexture
{
 public:
  /**
   * Attempts to load a texture.
   *
   * \note
   * The texture will actually be loaded to memory to deduce the texture size.
   *
   * \param path The texture path.
   *
   * \return
   * A texture if successful; an error code otherwise.
   */
  [[nodiscard]]
  static auto load(Path path) -> Result<NullTexture>;

  [[nodiscard]]
  auto get_handle() const -> void* override;

  [[nodiscard]]
  auto get_size() const -> TextureSize override;

  [[nodiscard]]
  auto get_path() const -> const Path& override;

 private:
  TextureSize mSize;
  Path mPath;

  NullTexture(TextureSize size, Path path);
};

}  // namespace tactile
