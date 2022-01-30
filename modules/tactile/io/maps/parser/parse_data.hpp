#pragma once

#include <filesystem>  // path

#include "io/maps/ir.hpp"
#include "parse_error.hpp"
#include "tactile_def.hpp"

namespace tactile::parsing {

class parse_data final {
 public:
  void set_path(std::filesystem::path path);

  void set_error(parse_error error);

  [[nodiscard]] auto path() const -> const std::filesystem::path&;

  [[nodiscard]] auto error() const -> parse_error;

  [[nodiscard]] auto data() -> map_data&;

  [[nodiscard]] auto data() const -> const map_data&;

 private:
  std::filesystem::path mPath;
  map_data mData;
  parse_error mError{parse_error::none};
};

}  // namespace tactile::parsing