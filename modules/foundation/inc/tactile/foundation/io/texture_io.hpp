// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Represents a loaded image resource.
 */
struct TextureData final {
  Managed<uchar> pixels;
  Int2 size;
};

/**
 * Attempts to load a texture from disk.
 *
 * \param path The file path to the image.
 *
 * \return
 *    The loaded texture.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto load_texture(const FilePath& path) -> Result<TextureData>;

}  // namespace tactile
