#pragma once

#include <filesystem>  // path

#include "common/ints.hpp"
#include "common/maybe.hpp"
#include "core/components/texture.hpp"

namespace Tactile {

void UnloadTextures();

[[nodiscard]] auto LoadTexture(const std::filesystem::path& path) -> Maybe<Texture>;

}  // namespace Tactile
