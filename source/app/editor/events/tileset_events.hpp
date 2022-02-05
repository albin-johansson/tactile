#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "core/region.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct ShowAddTilesetDialogEvent final
{};

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

struct [[deprecated]] InspectTilesetEvent final
{
  tileset_id id;
};

/// \brief Emitted when the name of a tileset should be changed.
struct SetTilesetNameEvent final
{
  tileset_id id{};   ///< ID of target tileset.
  std::string name;  ///< The new tileset name.
};

/// \} End of group events

}  // namespace tactile
