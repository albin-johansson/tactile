#pragma once

#include <filesystem>  // path

#include "parse_data.hpp"

namespace tactile::parsing {

class map_parser final {
 public:
  void parse_map(const std::filesystem::path& path);

  [[nodiscard]] auto data() const -> const parse_data&;

  [[nodiscard]] auto error() const -> parse_error;

  [[nodiscard]] auto is_okay() const -> bool;

 private:
  parsing::parse_data mData;
};

}  // namespace tactile::parsing