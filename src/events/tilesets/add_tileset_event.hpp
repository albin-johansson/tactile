#pragma once

#include <filesystem>  // path

namespace Tactile {

/// \addtogroup events
/// \{

struct AddTilesetEvent final
{
  std::filesystem::path path;
  int tile_width{};
  int tile_height{};
};

/// \} End of group events

}  // namespace Tactile
