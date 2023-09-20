// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/format/file_format_error.hpp"
#include "tactile/core/functional/expected.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/container/string.hpp"

namespace tactile {

/**
 * \interface IFileFormat
 *
 * \brief Interface for file format reader/parser implementations.
 *
 * \details File format implementations aren't required to support both save and load
 *          operations, and may simply report an error instead. However, any useful
 *          implementation would of course provide at least one of these operations.
 */
class TACTILE_CORE_API IFileFormat {
 public:
  TACTILE_INTERFACE_CLASS(IFileFormat);

  /**
   * \brief Attempts to load a map file.
   *
   * \param map_file the path to the map file.
   *
   * \return the loaded map; or an error code if something went wrong.
   */
  [[nodiscard]] virtual auto load_map(const fs::Path& map_file) const
      -> Expected<ir::Map, FileFormatError> = 0;

  /**
   * \brief Attempts to load a standalone tileset file.
   *
   * \param tileset_file the path to the tileset file.
   *
   * \return the loaded tileset; or an error code if something went wrong.
   */
  [[nodiscard]] virtual auto load_tileset(const fs::Path& tileset_file) const
      -> Expected<ir::Tileset, FileFormatError> = 0;

  virtual void save_map(const fs::Path& map_file, const ir::Map& map) = 0;

  virtual void save_tileset(const fs::Path& tileset_file, const ir::Tileset& tileset) = 0;

  /**
   * \brief Indicates whether a file extension is usable with the file format.
   *
   * \param extension a file extension, such as ".txt".
   *
   * \return true if the extension is valid; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_valid_extension(NativeStringView extension) const -> bool = 0;
};

}  // namespace tactile