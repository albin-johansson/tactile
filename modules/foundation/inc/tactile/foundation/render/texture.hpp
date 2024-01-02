// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Interface for texture resources.
 */
class ITexture {
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
  virtual auto get_size() const -> Int2 = 0;

  /**
   * Returns the file path used to load the texture.
   *
   * \return
   *    A file path.
   */
  [[nodiscard]]
  virtual auto get_path() const -> const FilePath& = 0;
};

}  // namespace tactile
