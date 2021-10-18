#pragma once

#include <filesystem>  // path

#include "core/region.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

/// \addtogroup events
/// \{

struct AddTilesetEvent final
{
  std::filesystem::path path;
  int tile_width{};
  int tile_height{};
};

struct RemoveTilesetEvent final
{
  TilesetID id;
};

struct SelectTilesetEvent final
{
  TilesetID id;
};

struct SetTilesetSelectionEvent final
{
  Region selection;
};

struct ShowTilesetPropertiesEvent final
{
  TilesetID id;
};

/// \} End of group events

}  // namespace Tactile
