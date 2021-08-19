#pragma once

#include <filesystem>  // path
#include <json.hpp>    // json

namespace Tactile {

[[nodiscard]] auto ReadJson(const std::filesystem::path& path) -> nlohmann::json;

}  // namespace Tactile
