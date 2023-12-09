// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/texture_io.hpp"

#include <utility>  // move

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace tactile {

auto load_texture(const FilePath& path) -> Maybe<TextureData>
{
  Managed<uchar> pixels {nullptr, [](uchar* ptr) noexcept { stbi_image_free(ptr); }};
  Int2 size {};

  const auto path_str = path.string();
  pixels.reset(stbi_load(path_str.c_str(), &size.x, &size.y, nullptr, STBI_rgb_alpha));

  if (pixels == nullptr) {
    return kNothing;
  }

  return TextureData {.pixels = std::move(pixels), .size = size};
}

}  // namespace tactile
