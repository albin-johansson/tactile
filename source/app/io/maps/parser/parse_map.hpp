#pragma once

#include <filesystem>  // path

#include "parse_data.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_map(const std::filesystem::path& path) -> parse_data;

}  // namespace tactile::parsing