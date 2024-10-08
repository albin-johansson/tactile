// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <unordered_map>  // unordered_map

#include <nlohmann/json.hpp>

#include "tactile/base/id.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class ITilesetView;

/**
 * Provides information about an external TMJ tileset.
 */
struct TmjFormatExternalTilesetData final
{
  /** The file path to the external tileset file. */
  std::filesystem::path path;

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
    std::unordered_map<TileID, TmjFormatExternalTilesetData>& external_tilesets)
    -> nlohmann::json;

}  // namespace tactile
