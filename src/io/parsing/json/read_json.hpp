#pragma once

#include <filesystem>  // path

#include "aliases/json.hpp"
#include "aliases/maybe.hpp"

namespace Tactile {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> Maybe<JSON>;

}  // namespace Tactile
