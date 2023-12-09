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
 * \brief Manages map/tileset save format parser/emitter implementations.
 */
class TACTILE_FOUNDATION_API SaveFormatManager final {
 public:
  /**
   * \brief Returns the global save format manager.
   *
   * \return a save format manager.
   */
  [[nodiscard]]
  static auto get() noexcept -> SaveFormatManager&;

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
   * \param tileset_path the tileset file path.
   * \param tileset      the tileset that will be saved.
   * \param options      the write options.
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
  void remove_format(ISaveFormat* format) noexcept;

 private:
  Vector<ISaveFormat*> mFormats;
};

}  // namespace tactile
