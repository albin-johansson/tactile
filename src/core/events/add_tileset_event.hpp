#pragma once

#include <filesystem>  // path

namespace tactile {

struct AddTilesetEvent final
{
  std::filesystem::path path;
  int tile_width{};
  int tile_height{};
};

}  // namespace tactile
