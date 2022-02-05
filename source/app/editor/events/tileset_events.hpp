#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "core/region.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct show_add_tileset_dialog_event final
{};

struct add_tileset_event final
{
  std::filesystem::path path;
  int32 tile_width{};
  int32 tile_height{};
};

struct remove_tileset_event final
{
  tileset_id id{};
};

struct select_tileset_event final
{
  tileset_id id{};
};

struct set_tileset_selection_event final
{
  Region selection;
};

/**
 * \brief Emitted when the name of a tileset should be changed.
 */
struct set_tileset_name_event final
{
  tileset_id id{};   ///< ID of target tileset.
  std::string name;  ///< The new tileset name.
};

/// \} End of group events

}  // namespace tactile
