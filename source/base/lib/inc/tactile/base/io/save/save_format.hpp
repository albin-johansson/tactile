// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>       // expected
#include <filesystem>     // path
#include <string>         // string
#include <system_error>   // error_code
#include <unordered_map>  // unordered_map

#include "tactile/base/container/string_map.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IMapView;

using SaveFormatExtraSettings = StringMap<std::string>;

/**
 * Provides save format parse options.
 */
struct SaveFormatReadOptions final
{
  /** Used for implementation-specific settings. */
  SaveFormatExtraSettings extra;

  /** The parent directory of the map or tileset file. */
  std::filesystem::path base_dir;

  /** Whether strict parsing is to be enforced. */
  bool strict_mode : 1;
};

/**
 * Provides save format save options.
 */
struct SaveFormatWriteOptions final
{
  /** Used for implementation-specific settings. */
  SaveFormatExtraSettings extra;

  /** The parent directory of the map or tileset file. */
  std::filesystem::path base_dir;

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
  virtual auto load_map(const std::filesystem::path& map_path,
                        const SaveFormatReadOptions& options) const
      -> std::expected<ir::Map, std::error_code> = 0;

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
  virtual auto save_map(const IMapView& map, const SaveFormatWriteOptions& options) const
      -> std::expected<void, std::error_code> = 0;
};

}  // namespace tactile
