#pragma once

#include <filesystem>  // path

#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"
#include "texture_info.hpp"

namespace Tactile {

void UnloadTextures();

[[nodiscard]] auto LoadTexture(const std::filesystem::path& path)
    -> Maybe<TextureInfo>;

}  // namespace Tactile
