#pragma once

#include <filesystem>  // path

#include "aliases/json.hpp"

namespace Tactile {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> JSON;

}  // namespace Tactile
