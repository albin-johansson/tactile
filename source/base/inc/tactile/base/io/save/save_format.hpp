// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IMapView;

/**
 * Provides save format parse options.
 */
struct SaveFormatReadOptions final
{
  /** The parent directory of the map or tileset file. */
  Path base_dir;

  /** Whether strict parsing is to be enforced. */
  bool strict_mode : 1;
};

/**
 * Provides save format save options.
 */
struct SaveFormatWriteOptions final
{
  /** The parent directory of the map or tileset file. */
  Path base_dir;

  /** Whether tilesets are saved in separate files. */
  bool use_external_tilesets : 1;

  /** Whether the output is indented. */
  bool use_indentation : 1;

  /** Whether tile rows are aligned by row. */
  bool fold_tile_layer_data : 1;
};

/**
 * Interface for save file format reader/parser implementations.
 *
 * \details
 * Save format implementations aren't required to support both save and load
 * operations, and may simply report an error instead. However, a useful
 * implementation would of course provide at least one of these operations.
 */
class ISaveFormat
{
 public:
  TACTILE_INTERFACE_CLASS(ISaveFormat);

  /**
   * Attempts to load a map file.
   *
   * \param map_path The path to the map file.
   * \param options  The read options.
   *
   * \return
   * An intermediate map if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto load_map(const Path& map_path,
                        const SaveFormatReadOptions& options) const
      -> Result<ir::Map> = 0;

  /**
   * Attempts to save a map.
   *
   * \param map     The map that will be saved.
   * \param options The write options.
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto save_map(const IMapView& map,
                        const SaveFormatWriteOptions& options) const
      -> Result<void> = 0;
};

}  // namespace tactile
