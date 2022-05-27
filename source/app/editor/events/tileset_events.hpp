/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/uuid.hpp"
#include "core/region.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct ShowTilesetCreationDialogEvent final
{};

struct AddTilesetEvent final
{
  std::filesystem::path path;
  int32 tile_width{};
  int32 tile_height{};
};

struct RemoveTilesetEvent final
{
  UUID tileset_id{};
};

struct SelectTilesetEvent final
{
  UUID tileset_id{};
};

struct SetTilesetSelectionEvent final
{
  Region selection;
};

/**
 * \brief Emitted when the name of a tileset should be changed.
 */
struct SetTilesetNameEvent final
{
  TilesetID id{};    ///< ID of target tileset.
  std::string name;  ///< The new tileset name.
};

/// \} End of group events

}  // namespace tactile
