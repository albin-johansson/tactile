#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"

namespace tactile::IO {

void SaveJson(const JSON& json, const std::filesystem::path& path, bool indent);

}  // namespace tactile::IO
