// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_id.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Interface for save file format reader/parser implementations.
 *
 * \details 
 *    Save format implementations aren't required to support both save and load
 *    operations, and may simply report an error instead. However, any useful
 *    implementation would of course provide at least one of these operations.
 */
class ISaveFormat {
 public:
  TACTILE_INTERFACE_CLASS(ISaveFormat);

  /**
   * Attempts to load a map file.
   *
   * \param map_path The path to the map file.
   * \param options  The read options.
   *
   * \return
   *    The loaded map, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto load_map(const FilePath& map_path,
                        const SaveFormatReadOptions& options) const
      -> Result<ir::Map> = 0;

  /**
   * Attempts to load a standalone tileset file.
   *
   * \param tileset_path The path to the tileset file.
   * \param options      The read options.
   *
   * \return
   *    The loaded tileset, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto load_tileset(const FilePath& tileset_path,
                            const SaveFormatReadOptions& options) const
      -> Result<ir::Tileset> = 0;

  /**
   * Attempts to save a map.
   *
   * \param map_path The map file path.
   * \param map      The map that will be saved.
   * \param options  The write options.
   *
   * \return
   *    Nothing on success, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto save_map(const FilePath& map_path,
                        const ir::Map& map,
                        const SaveFormatWriteOptions& options) const -> Result<void> = 0;

  /**
   * Attempts to save a tileset.
   *
   * \param tileset_path The tileset file path.
   * \param tileset      The tileset that will be saved.
   * \param options      The write options.
   *
   * \return
   *    Nothing on success, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto save_tileset(const FilePath& tileset_path,
                            const ir::Tileset& tileset,
                            const SaveFormatWriteOptions& options) const
      -> Result<void> = 0;

  /**
   * Indicates whether a file extension is usable with the save format.
   *
   * \param extension A file extension, such as ".txt".
   *
   * \return
   *    True if the extension is valid; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_valid_extension(NativeStringView extension) const -> bool = 0;

  /**
   * Returns the identifier associated with the save format.
   *
   * \return
   *    A save format identifier.
   */
  [[nodiscard]]
  virtual auto id() const -> SaveFormatId = 0;
};

}  // namespace tactile