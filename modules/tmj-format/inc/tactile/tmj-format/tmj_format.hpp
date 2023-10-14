// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/io/save/save_format.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"

namespace tactile::tmj {

/**
 * \brief Provides import and export support for the Tiled JSON format.
 *
 * \details This format uses ".tmj" or ".json" file extensions.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format
 *
 * \ingroup TMJ
 */
class TmjFormat final : public ISaveFormat {
 public:
  [[nodiscard]]
  TACTILE_TMJ_API auto load_map(const FilePath& map_path,
                                const SaveFormatReadOptions& options) const
      -> Result<ir::Map> override;

  [[nodiscard]]
  TACTILE_TMJ_API auto load_tileset(const FilePath& tileset_path,
                                    const SaveFormatReadOptions& options) const
      -> Result<ir::Tileset> override;

  [[nodiscard]]
  TACTILE_TMJ_API auto save_map(const FilePath& map_path,
                                const ir::Map& map,
                                const SaveFormatWriteOptions& options) const
      -> Result<void> override;

  [[nodiscard]]
  TACTILE_TMJ_API auto save_tileset(const FilePath& tileset_path,
                                    const ir::Tileset& tileset,
                                    const SaveFormatWriteOptions& options) const
      -> Result<void> override;

  [[nodiscard]]
  TACTILE_TMJ_API auto is_valid_extension(NativeStringView extension) const
      -> bool override;
};

}  // namespace tactile::tmj