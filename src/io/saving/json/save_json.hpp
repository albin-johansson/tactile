#pragma once

#include <filesystem>  // path
#include <json.hpp>    // json

namespace Tactile::IO {

void SaveJson(const nlohmann::json& json, const std::filesystem::path& path);

}  // namespace Tactile::IO
