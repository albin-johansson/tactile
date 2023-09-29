// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

TACTILE_FWD(class ISaveFormat)

/**
 * \brief Manages map/tileset save format parser/emitter implementations.
 */
class SaveFormatManager final {
 public:
  /**
   * \brief Returns the global save format manager.
   *
   * \return a save format manager.
   */
  [[nodiscard]]
  TACTILE_CORE_API static auto get() -> SaveFormatManager&;

  /**
   * \brief Loads a map file using one of the available save format handlers.
   *
   * \param map_file the map file path.
   *
   * \return the map data, or an error code if something went wrong.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto load_map(const fs::Path& map_file) const -> Result<ir::Map>;

  /**
   * \brief Loads a tileset file using one of the available save format handlers.
   *
   * \param tileset_file the tileset file path.
   *
   * \return the tileset data, or an error code if something went wrong.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto load_tileset(const fs::Path& tileset_file) const
      -> Result<ir::Tileset>;

  /**
   * \brief Registers a save format handler.
   *
   * \param format the save format handler.
   */
  TACTILE_CORE_API void add_format(ISaveFormat* format);

  /**
   * \brief Removes a save format handler.
   *
   * \param format the save format handler.
   */
  TACTILE_CORE_API void remove_format(ISaveFormat* format);

 private:
  Vector<ISaveFormat*> mFormats;
};

}  // namespace tactile
