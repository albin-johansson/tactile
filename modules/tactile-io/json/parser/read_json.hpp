#pragma once

#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> Maybe<JSON>;

}  // namespace Tactile::IO
