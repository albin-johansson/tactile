// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path
#include <fstream>     // ifstream
#include <ios>         // ios
#include <iterator>    // istreambuf_iterator
#include <optional>    // optional

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Reads an entire binary file from disk.
 *
 * \param path The file path to the file to load.
 *
 * \return
 * The file content if successful; an empty optional otherwise.
 */
inline auto read_binary_file(const std::filesystem::path& path) -> std::optional<String>
{
  std::ifstream stream {path, std::ios::in | std::ios::binary};

  if (stream.good()) {
    return String {std::istreambuf_iterator<char> {stream}, std::istreambuf_iterator<char> {}};
  }

  return std::nullopt;
}

}  // namespace tactile
