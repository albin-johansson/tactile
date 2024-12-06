// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace tactile::common {

/// Reads a binary file from disk.
auto read_binary_file(const std::filesystem::path& path) -> std::optional<std::string>;

}  // namespace tactile::common
