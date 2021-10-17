#pragma once

#include <filesystem>  // path

#include "common/maybe.hpp"
#include "io/json.hpp"

namespace Tactile {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> Maybe<JSON>;

}  // namespace Tactile
