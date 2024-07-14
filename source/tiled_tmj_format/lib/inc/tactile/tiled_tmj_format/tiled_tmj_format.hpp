// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

namespace tactile {

/**
 * Implements the Tiled TMJ save format.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/
 */
class TACTILE_TMJ_FORMAT_API TiledTmjFormat final : public ISaveFormat
{
 public:
  [[nodiscard]]
  auto load_map(const Path& map_path,  //
                const SaveFormatReadOptions& options) const
      -> Result<ir::Map> override;

  [[nodiscard]]
  auto save_map(const IMapView& map,  //
                const SaveFormatWriteOptions& options) const
      -> Result<void> override;
};

}  // namespace tactile
