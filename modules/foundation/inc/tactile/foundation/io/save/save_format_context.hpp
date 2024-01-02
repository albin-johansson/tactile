// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Manages map/tileset save format parser/emitter implementations.
 */
class TACTILE_FOUNDATION_API SaveFormatContext final {
 public:
  /**
   * Returns the global save format manager.
   *
   * \return
   *    A save format manager.
   */
  [[nodiscard]]
  static auto get() noexcept -> SaveFormatContext&;

  /**
   * Loads a map file using one of the available save format handlers.
   *
   * \param map_path The map file path.
   * \param options  The read options.
   *
   * \return
   *    The map data, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto load_map(const FilePath& map_path, const SaveFormatReadOptions& options) const
      -> Result<ir::Map>;

  /**
   * Loads a tileset file using one of the available save format handlers.
   *
   * \param tileset_path The tileset file path.
   * \param options      The read options.
   *
   * \return
   *    The tileset data, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto load_tileset(const FilePath& tileset_path,
                    const SaveFormatReadOptions& options) const -> Result<ir::Tileset>;

  /**
   * Saves a map using one of the available save format handlers.
   *
   * \param save_format_id The save format to use.
   * \param map_path       The map file path.
   * \param map            The map that will be saved.
   * \param options        The write options.
   *
   * \return
   *    Nothing on success, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto save_map(SaveFormatId save_format_id,
                const FilePath& map_path,
                const ir::Map& map,
                const SaveFormatWriteOptions& options) const -> Result<void>;

  /**
   * Saves a tileset using one of the available save format handlers.
   *
   * \param save_format_id The save format to use.
   * \param tileset_path   The tileset file path.
   * \param tileset        The tileset that will be saved.
   * \param options        The write options.
   *
   * \return
   *    Nothing on success, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto save_tileset(SaveFormatId save_format_id,
                    const FilePath& tileset_path,
                    const ir::Tileset& tileset,
                    const SaveFormatWriteOptions& options) const -> Result<void>;

  /**
   * Registers a save format handler.
   *
   * \param format The save format handler.
   */
  void add_format(ISaveFormat* format);

  /**
   * Removes a save format handler.
   *
   * \param format The save format handler.
   */
  void remove_format(ISaveFormat* format) noexcept;

 private:
  Vector<ISaveFormat*> mFormats;

  [[nodiscard]]
  auto _find_format(SaveFormatId id) const -> const ISaveFormat*;
};

}  // namespace tactile
