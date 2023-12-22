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
 * \interface ISaveFormat
 *
 * \brief Interface for save file format reader/parser implementations.
 *
 * \details Save format implementations aren't required to support both save and load
 *          operations, and may simply report an error instead. However, any useful
 *          implementation would of course provide at least one of these operations.
 */
class ISaveFormat {
 public:
  TACTILE_INTERFACE_CLASS(ISaveFormat);

  /**
   * \brief Attempts to load a map file.
   *
   * \param map_path the path to the map file.
   * \param options  the read options.
   *
   * \return the loaded map, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto load_map(const FilePath& map_path,
                        const SaveFormatReadOptions& options) const
      -> Result<ir::Map> = 0;

  /**
   * \brief Attempts to load a standalone tileset file.
   *
   * \param tileset_path the path to the tileset file.
   * \param options      the read options.
   *
   * \return the loaded tileset, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto load_tileset(const FilePath& tileset_path,
                            const SaveFormatReadOptions& options) const
      -> Result<ir::Tileset> = 0;

  /**
   * \brief Attempts to save a map.
   *
   * \param map_path the map file path.
   * \param map      the map that will be saved.
   * \param options  the write options.
   *
   * \return nothing on success, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto save_map(const FilePath& map_path,
                        const ir::Map& map,
                        const SaveFormatWriteOptions& options) const -> Result<void> = 0;

  /**
   * \brief Attempts to save a tileset.
   *
   * \param tileset_path the tileset file path.
   * \param tileset      the tileset that will be saved.
   * \param options      the write options.
   *
   * \return nothing on success, or an error code on failure.
   */
  [[nodiscard]]
  virtual auto save_tileset(const FilePath& tileset_path,
                            const ir::Tileset& tileset,
                            const SaveFormatWriteOptions& options) const
      -> Result<void> = 0;

  /**
   * \brief Indicates whether a file extension is usable with the save format.
   *
   * \param extension a file extension, such as ".txt".
   *
   * \return true if the extension is valid; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_valid_extension(NativeStringView extension) const -> bool = 0;

  /**
   * \brief Returns the identifier associated with the save format.
   *
   * \return a save format identifier.
   */
  [[nodiscard]]
  virtual auto id() const -> SaveFormatId = 0;
};

}  // namespace tactile