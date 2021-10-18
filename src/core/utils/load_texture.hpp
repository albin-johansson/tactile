#pragma once

#include <filesystem>  // path

#include "core/components/texture.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

void UnloadTextures();

[[nodiscard]] auto LoadTexture(const std::filesystem::path& path) -> Maybe<Texture>;

}  // namespace Tactile
