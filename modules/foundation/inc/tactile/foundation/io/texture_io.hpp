// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a loaded image resource.
 */
struct TextureData final {
  Managed<uchar> pixels;
  Int2 size;
};

/**
 * \brief Attempts to load a texture from disk.
 *
 * \param path the file path to the image.
 *
 * \return the loaded texture.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto load_texture(const FilePath& path) -> Maybe<TextureData>;

}  // namespace tactile
