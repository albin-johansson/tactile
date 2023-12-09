// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Interface for texture resources.
 */
class ITexture {
 public:
  TACTILE_INTERFACE_CLASS(ITexture);

  /**
   * \brief Returns a handle to the underlying resource handle.
   *
   * \return an opaque resource handle.
   */
  [[nodiscard]]
  virtual auto get_handle() const -> void* = 0;

  /**
   * \brief Returns the size of the texture.
   *
   * \return a texture size.
   */
  [[nodiscard]]
  virtual auto get_size() const -> Int2 = 0;

  /**
   * \brief Returns the file path used to load the texture.
   *
   * \return a file path.
   */
  [[nodiscard]]
  virtual auto get_path() const -> const FilePath& = 0;
};

}  // namespace tactile