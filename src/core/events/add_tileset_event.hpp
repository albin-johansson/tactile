#pragma once

#include <filesystem>  // path

namespace Tactile {

struct AddTilesetEvent final
{
  std::filesystem::path path;
  int tile_width{};
  int tile_height{};
};

}  // namespace Tactile
