#pragma once

#include <filesystem>  // path

#include "aliases/tileset_id.hpp"
#include "core/region.hpp"

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
  tileset_id id;
};

struct SelectTilesetEvent final
{
  tileset_id id;
};

struct SetTilesetSelectionEvent final
{
  Region selection;
};

/// \} End of group events

}  // namespace Tactile
