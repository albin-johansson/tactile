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

#include "texture_loader.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <fmt/std.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>

namespace tactile {

void TextureDataDeleter::operator()(uchar* data) noexcept
{
  stbi_image_free(data);
}

auto load_texture_data(const Path& path) -> Maybe<TextureData>
{
  TextureData texture_data;
  texture_data.pixels.reset(stbi_load(path.string().c_str(),
                                      &texture_data.size.x,
                                      &texture_data.size.y,
                                      nullptr,
                                      STBI_rgb_alpha));

  if (!texture_data.pixels) {
    spdlog::error("[IO] Could not load texture at {}", path);
    return nothing;
  }

  return texture_data;
}

}  // namespace tactile
