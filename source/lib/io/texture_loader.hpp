/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/type/path.hpp"
#include "tactile/core/containers/smart_ptr.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "core/math/vector.hpp"
#include "core/prelude.hpp"

namespace tactile {

struct TextureDataDeleter final {
  void operator()(uchar* data) noexcept;
};

using TexturePixelData = UniquePtr<uchar, TextureDataDeleter>;

/// Represents a loaded image file.
// TODO add ImageChannels enum
struct TextureData final {
  TexturePixelData pixels;  ///< The image pixel data.
  Int2 size {};             ///< The image dimensions.
};

/**
 * Loads raw texture data from an image file on disk.
 *
 * \param path the file path to the texture file.
 *
 * \return the loaded texture data, or nothing if something went wrong.
 */
[[nodiscard]] auto load_texture_data(const Path& path) -> Maybe<TextureData>;

}  // namespace tactile
