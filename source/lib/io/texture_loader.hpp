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

#include "common/primitives.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/path.hpp"
#include "common/type/ptr.hpp"

namespace tactile {

struct TextureDataDeleter final {
  void operator()(uchar* data) noexcept;
};

using TexturePixelData = Unique<uchar, TextureDataDeleter>;

/// Represents a loaded image file.
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
