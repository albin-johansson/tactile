#pragma once

#include <GL/glew.h>

#include <filesystem>  // path

#include "aliases/maybe.hpp"

namespace tactile {

struct TextureInfo final
{
  GLuint texture{};
  GLint width{};
  GLint height{};
};

[[nodiscard]] auto LoadTexture(const std::filesystem::path& path)
    -> Maybe<TextureInfo>;

}  // namespace tactile
