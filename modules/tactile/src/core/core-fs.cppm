// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <filesystem>

export module tactile.core:fs;

export namespace tactile {

using Path = std::filesystem::path;

namespace fs = std::filesystem;

}  // namespace tactile
