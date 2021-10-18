#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"

namespace Tactile::IO {

void SaveJson(const JSON& json, const std::filesystem::path& path);

}  // namespace Tactile::IO
