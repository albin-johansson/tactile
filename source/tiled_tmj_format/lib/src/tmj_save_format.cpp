// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_save_format.hpp"

#include <system_error>  // errc, make_error_code

#include "tactile/base/document/map_view.hpp"
#include "tactile/json_util/json_io.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj_format/tmj_format_map_parser.hpp"
#include "tactile/tiled_tmj_format/tmj_format_save_visitor.hpp"

namespace tactile {

TmjSaveFormat::TmjSaveFormat(IRuntime* runtime)
  : mRuntime {runtime}
{}

auto TmjSaveFormat::load_map(const Path& map_path,
                             const SaveFormatReadOptions& options) const -> Result<ir::Map>
{
  log(LogLevel::kDebug, "Loading TMJ map from {}", map_path.string());

  const auto map_json = load_json(map_path);
  if (!map_json.has_value()) {
    return unexpected(std::make_error_code(std::errc::io_error));
  }

  return parse_tiled_tmj_map(*mRuntime, *map_json, options)
      .transform_error([](const SaveFormatParseError error) {
        log(LogLevel::kError, "TMJ parse error: {}", to_string(error));
        return std::make_error_code(std::errc::io_error);
      });
}

auto TmjSaveFormat::save_map(const IMapView& map,
                             const SaveFormatWriteOptions& options) const -> Result<void>
{
  const auto* map_path = map.get_path();

  if (!map_path) {
    log(LogLevel::kError, "Map has no associated file path");
    return unexpected(std::make_error_code(std::errc::invalid_argument));
  }

  log(LogLevel::kDebug, "Saving TMJ map to {}", map_path->string());

  TmjFormatSaveVisitor visitor {mRuntime, options};

  return map.accept(visitor)
      .and_then([&] {
        const auto& map_json = visitor.get_map_json();
        return save_json(*map_path, map_json, options.use_indentation ? 2 : 0);
      })
      .and_then([&]() -> Result<void> {
        const auto& external_tilesets = visitor.get_external_tilesets();

        for (const auto& [first_tiled_id, external_tileset] : external_tilesets) {
          const auto save_tileset_result = save_json(external_tileset.path,
                                                     external_tileset.json,
                                                     options.use_indentation ? 2 : 0);
          if (!save_tileset_result.has_value()) {
            log(LogLevel::kError, "Could not save external tileset");
            return propagate_unexpected(save_tileset_result);
          }
        }

        return kOK;
      });
}

}  // namespace tactile
