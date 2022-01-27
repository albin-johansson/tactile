#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include <tactile_def.hpp>

#include "core/components/texture.hpp"

namespace tactile {

class TextureManager final {
 public:
  TACTILE_DEFAULT_COPY(TextureManager)
  TACTILE_DEFAULT_MOVE(TextureManager)

  TextureManager() = default;

  ~TextureManager();

  [[nodiscard]] auto Load(const std::filesystem::path& path) -> Maybe<comp::texture>;

 private:
  std::vector<uint> mTextures;
};

}  // namespace tactile
