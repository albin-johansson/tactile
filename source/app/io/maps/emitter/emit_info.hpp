#pragma once

#include <filesystem>  // path

#include "io/maps/ir.hpp"
#include "tactile.hpp"

namespace tactile::emitter {

class emit_info final {
 public:
  explicit emit_info(std::filesystem::path destination, ir::map_data data);

  [[nodiscard]] auto destination_file() const -> const std::filesystem::path&;

  [[nodiscard]] auto destination_dir() const -> const std::filesystem::path&;

  [[nodiscard]] auto data() const -> const ir::map_data&;

 private:
  std::filesystem::path mDestinationFile;
  std::filesystem::path mDestinationDir;
  ir::map_data mData;
};

}  // namespace tactile::emitter