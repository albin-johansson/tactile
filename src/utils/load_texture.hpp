#pragma once

#include <filesystem>  // path

#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"

namespace tactile {

struct TextureInfo final
{
  uint texture{};
  int width{};
  int height{};
};

[[nodiscard]] auto LoadTexture(const std::filesystem::path& path)
    -> Maybe<TextureInfo>;

}  // namespace tactile
