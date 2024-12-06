// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)
#include "tactile/common/io/file_streams.hpp"

#include <fstream>
#include <iterator>

namespace tactile::common {

auto read_binary_file(const std::filesystem::path& path) -> std::optional<std::string>
{
  std::ifstream stream {path, std::ios::in | std::ios::binary};

  if (stream.good()) {
    return std::string {std::istreambuf_iterator {stream}, std::istreambuf_iterator<char> {}};
  }

  return std::nullopt;
}

}  // namespace tactile::common
