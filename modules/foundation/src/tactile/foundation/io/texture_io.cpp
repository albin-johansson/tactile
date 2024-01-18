// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/texture_io.hpp"

#include <utility>  // move

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

auto load_texture(const FilePath& path) -> Result<TextureData>
{
  Managed<uchar> pixels {nullptr, [](uchar* ptr) noexcept { stbi_image_free(ptr); }};
  Int2 size {};

  const auto path_str = path.string();
  pixels.reset(stbi_load(path_str.c_str(),
                         &size[0],  // NOLINT(*-container-data-pointer)
                         &size[1],
                         nullptr,
                         STBI_rgb_alpha));

  if (pixels == nullptr) {
    TACTILE_LOG_ERROR("Could not load texture '{}'", path.string());
    return unexpected(make_generic_error(GenericError::kIOError));
  }

  return TextureData {.pixels = std::move(pixels), .size = size};
}

}  // namespace tactile
