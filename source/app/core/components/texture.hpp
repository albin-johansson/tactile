#pragma once

#include <filesystem>  // path

#include "tactile_def.hpp"

namespace tactile::comp {

/**
 * \brief Represents a loaded OpenGL texture.
 */
struct texture final
{
  uint id{};                   ///< The OpenGL texture identifier.
  int width{};                 ///< The texture pixel width.
  int height{};                ///< The texture pixel height.
  std::filesystem::path path;  ///< Source file path.
};

}  // namespace tactile::comp