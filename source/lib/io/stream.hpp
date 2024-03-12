// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fstream>  // ifstream, ofstream

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"

namespace tactile {

enum class FileType {
  Text,
  Binary
};

[[nodiscard]] auto open_input_stream(const char* path, FileType type)
    -> Maybe<std::ifstream>;

[[nodiscard]] auto open_input_stream(const Path& path, FileType type)
    -> Maybe<std::ifstream>;

[[nodiscard]] auto open_output_stream(const char* path, FileType type)
    -> Maybe<std::ofstream>;

[[nodiscard]] auto open_output_stream(const Path& path, FileType type)
    -> Maybe<std::ofstream>;

}  // namespace tactile
