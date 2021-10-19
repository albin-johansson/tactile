#pragma once

#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

#include "core/components/texture.hpp"

namespace Tactile {

void UnloadTextures();

[[nodiscard]] auto LoadTexture(const std::filesystem::path& path) -> Maybe<Texture>;

}  // namespace Tactile
