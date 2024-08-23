// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/null_renderer/null_texture.hpp"

#include <system_error>  // errc, make_error_code
#include <utility>       // move

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "tactile/runtime/logging.hpp"

namespace tactile {

NullTexture::NullTexture(const TextureSize size, Path path) :
  mSize {size},
  mPath {std::move(path)}
{}

auto NullTexture::load(Path path) -> std::expected<NullTexture, std::error_code>
{
  TextureSize size {};

  const auto path_string = path.string();
  auto* pixels =
      stbi_load(path_string.c_str(), &size.width, &size.height, nullptr, STBI_default);

  if (!pixels) {
    log(LogLevel::kError,
        "Could not load texture '{}': {}",
        path_string,
        stbi_failure_reason());
    return std::unexpected {std::make_error_code(std::errc::io_error)};
  }

  stbi_image_free(pixels);

  return NullTexture {size, std::move(path)};
}

auto NullTexture::get_handle() const -> void*
{
  return nullptr;
}

auto NullTexture::get_size() const -> TextureSize
{
  return mSize;
}

auto NullTexture::get_path() const -> const Path&
{
  return mPath;
}

}  // namespace tactile
