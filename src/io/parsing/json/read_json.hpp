#pragma once

#include <filesystem>  // path

#include "common/json.hpp"
#include "common/maybe.hpp"

namespace Tactile {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> Maybe<JSON>;

}  // namespace Tactile
