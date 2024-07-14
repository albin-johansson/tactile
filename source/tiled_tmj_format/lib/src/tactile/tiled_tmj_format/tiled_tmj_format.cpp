// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tiled_tmj_format.hpp"

#include <system_error>  // errc, make_error_code

namespace tactile {

auto TiledTmjFormat::load_map(const Path& map_path,
                              const SaveFormatReadOptions& options) const
    -> Result<ir::Map>
{
  (void) map_path;
  (void) options;
  return unexpected(std::make_error_code(std::errc::operation_not_supported));
}

auto TiledTmjFormat::save_map(const IMapView& map,
                              const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  (void) map;
  (void) options;
  return unexpected(std::make_error_code(std::errc::operation_not_supported));
}

}  // namespace tactile
