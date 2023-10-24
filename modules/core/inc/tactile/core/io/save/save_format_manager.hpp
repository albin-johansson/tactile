// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "save_format.hpp"
#include "save_format_options.hpp"
#include "tactile/core/api.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Manages map/tileset save format parser/emitter implementations.
 */
class TACTILE_CORE_API SaveFormatManager final {
 public:
  /**
   * \brief Returns the global save format manager.
   *
   * \return a save format manager.
   */
  [[nodiscard]]
  static auto get() -> SaveFormatManager&;

  /**
   * \brief Loads a map file using one of the available save format handlers.
   *
   * \param map_path the map file path.
   * \param options  the read options.
   *
   * \return the map data, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto load_map(const FilePath& map_path, const SaveFormatReadOptions& options) const
      -> Result<ir::Map>;

  /**
   * \brief Loads a tileset file using one of the available save format handlers.
   *
   * \param tileset_path the tileset file path.
   * \param options      the read options.
   *
   * \return the tileset data, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto load_tileset(const FilePath& tileset_path,
                    const SaveFormatReadOptions& options) const -> Result<ir::Tileset>;

  /**
   * \brief Saves a map using one of the available save format handlers.
   *
   * \param map_path the map file path.
   * \param map      the map that will be saved.
   * \param options  the write options.
   *
   * \return nothing on success, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto save_map(const FilePath& map_path,
                const ir::Map& map,
                const SaveFormatWriteOptions& options) const -> Result<void>;

  /**
   * \brief Saves a tileset using one of the available save format handlers.
   *
   * \param map_path the tileset file path.
   * \param map      the tileset that will be saved.
   * \param options  the write options.
   *
   * \return nothing on success, or an error code if an error occurred.
   */
  [[nodiscard]]
  auto save_tileset(const FilePath& tileset_path,
                    const ir::Tileset& tileset,
                    const SaveFormatWriteOptions& options) const -> Result<void>;

  /**
   * \brief Registers a save format handler.
   *
   * \param format the save format handler.
   */
  void add_format(ISaveFormat* format);

  /**
   * \brief Removes a save format handler.
   *
   * \param format the save format handler.
   */
  void remove_format(ISaveFormat* format);

 private:
  Vector<ISaveFormat*> mFormats;
};

}  // namespace tactile
