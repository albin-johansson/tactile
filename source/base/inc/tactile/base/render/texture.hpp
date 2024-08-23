// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/** Represents the extent of a texture. */
struct TextureSize final
{
  int width;
  int height;
};

/** Interface for texture resources. */
class ITexture
{
 public:
  TACTILE_INTERFACE_CLASS(ITexture);

  /**
   * Returns a handle to the underlying resource handle.
   *
   * \return
   *    An opaque resource handle.
   */
  [[nodiscard]]
  virtual auto get_handle() const -> void* = 0;

  /**
   * Returns the size of the texture.
   *
   * \return
   *    A texture size.
   */
  [[nodiscard]]
  virtual auto get_size() const -> TextureSize = 0;

  /**
   * Returns the file path used to load the texture.
   *
   * \return
   *    A file path.
   */
  [[nodiscard]]
  virtual auto get_path() const -> const Path& = 0;
};

}  // namespace tactile
