#pragma once

#include <filesystem>  // path

#include "aliases/ints.hpp"

namespace Tactile {

struct TextureInfo final
{
  uint texture{};              ///< OpenGL texture identifier.
  int width{};                 ///< Texture width.
  int height{};                ///< Texture height.
  std::filesystem::path path;  ///< Source file path.
};

}  // namespace Tactile
