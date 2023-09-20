// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/format/file_format_error.hpp"
#include "tactile/core/functional/expected.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

TACTILE_FWD(class IFileFormat)

/**
 * \brief Manages map/tileset file format parser/emitter implementations.
 */
class FileFormatManager final {
 public:
  /** \brief Returns the global file format manager. */
  [[nodiscard]]
  TACTILE_CORE_API static auto get() -> FileFormatManager&;

  /**
   * \brief Loads a map file using one of the available file format handlers.
   *
   * \param map_file the map file path.
   *
   * \return the map data, or an error code if something went wrong.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto load_map(const fs::Path& map_file) const
      -> Expected<ir::Map, FileFormatError>;

  /**
   * \brief Loads a tileset file using one of the available file format handlers.
   *
   * \param tileset_file the tileset file path.
   *
   * \return the tileset data, or an error code if something went wrong.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto load_tileset(const fs::Path& tileset_file) const
      -> Expected<ir::Tileset, FileFormatError>;

  /**
   * \brief Registers a file format handler.
   *
   * \param format the file format handler.
   */
  TACTILE_CORE_API void add_format(IFileFormat* format);

  /**
   * \brief Removes a file format handler.
   *
   * \param format the file format handler.
   */
  TACTILE_CORE_API void remove_format(IFileFormat* format);

 private:
  Vector<IFileFormat*> mFormats;
};

}  // namespace tactile
