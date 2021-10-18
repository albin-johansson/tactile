#pragma once

#include <filesystem>  // path
#include <optional>    // optional

#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> std::optional<JSON>;

}  // namespace Tactile
