// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/math/vector.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a loaded image resource.
 */
struct TACTILE_CORE_API TextureData final {
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
TACTILE_CORE_API auto load_texture(const FilePath& path) -> Maybe<TextureData>;

}  // namespace tactile
