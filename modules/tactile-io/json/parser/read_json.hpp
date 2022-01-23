#pragma once

#include <filesystem>  // path

#include <tactile_def.hpp>

#include "../json_common.hpp"

namespace tactile::IO {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> Maybe<JSON>;

}  // namespace tactile::IO
