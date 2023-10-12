// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/format/save_format.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"

namespace tactile {

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
  TACTILE_TMJ_API auto load_map(const fs::Path& map_file) const
      -> Result<ir::Map> override;

  [[nodiscard]]
  TACTILE_TMJ_API auto load_tileset(const fs::Path& tileset_file) const
      -> Result<ir::Tileset> override;

  TACTILE_TMJ_API void save_map(const fs::Path& map_file, const ir::Map& map) override;

  TACTILE_TMJ_API void save_tileset(const fs::Path& tileset_file,
                                    const ir::Tileset& tileset) override;

  [[nodiscard]]
  TACTILE_TMJ_API auto is_valid_extension(NativeStringView extension) const
      -> bool override;
};

}  // namespace tactile