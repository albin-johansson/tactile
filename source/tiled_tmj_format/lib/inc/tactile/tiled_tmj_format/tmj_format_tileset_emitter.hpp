// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

namespace tactile {

class ITilesetView;

/**
 * Provides information about an external TMJ tileset.
 */
struct TmjFormatExternalTilesetData final
{
  /** The file path to the external tileset file. */
  Path path;

  /** The external tileset JSON. */
  nlohmann::json json;
};

/**
 * Emits a single Tiled TMJ tileset JSON node.
 *
 * \param      tileset           The view of the tileset.
 * \param      options           The write options to use.
 * \param[out] external_tilesets A collection of external tilesets. Only used if external
 *                               tilesets are enabled.
 *
 * \return
 * A tileset definition JSON node.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/#tileset
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_tiled_tmj_tileset(
    const ITilesetView& tileset,
    const SaveFormatWriteOptions& options,
    HashMap<TileID, TmjFormatExternalTilesetData>& external_tilesets) -> nlohmann::json;

}  // namespace tactile
