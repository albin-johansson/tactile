#pragma once

#include <filesystem>  // path

#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"
#include "texture_info.hpp"

namespace tactile {

[[nodiscard]] auto LoadTexture(const std::filesystem::path& path)
    -> Maybe<TextureInfo>;

}  // namespace tactile
