// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_save_format.hpp"

#include "tactile/base/document/map_view.hpp"
#include "tactile/json_util/json_io.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj/tmj_format_map_parser.hpp"
#include "tactile/tiled_tmj/tmj_format_save_visitor.hpp"

namespace tactile {

TmjSaveFormat::TmjSaveFormat(IRuntime* runtime)
  : mRuntime {runtime}
{}

auto TmjSaveFormat::load_map(const std::filesystem::path& map_path,
                             const SaveFormatReadOptions& options) const
    -> std::expected<ir::Map, ErrorCode>
{
  runtime::log(LogLevel::kDebug, "Loading TMJ map from {}", map_path.string());

  const auto map_json = load_json(map_path);
  if (!map_json.has_value()) {
    return std::unexpected {ErrorCode::kParseError};
  }

  return parse_tiled_tmj_map(*mRuntime, *map_json, options);
}

auto TmjSaveFormat::save_map(const IMapView& map, const SaveFormatWriteOptions& options) const
    -> std::expected<void, ErrorCode>
{
  const auto* map_path = map.get_path();

  if (!map_path) {
    runtime::log(LogLevel::kError, "Map has no associated file path");
    return std::unexpected {ErrorCode::kBadState};
  }

  runtime::log(LogLevel::kDebug, "Saving TMJ map to {}", map_path->string());

  TmjFormatSaveVisitor visitor {mRuntime, options};

  return map.accept(visitor)
      .and_then([&] {
        const auto& map_json = visitor.get_map_json();
        return save_json(*map_path, map_json, options.use_indentation ? 2 : 0);
      })
      .and_then([&]() -> std::expected<void, ErrorCode> {
        const auto& external_tilesets = visitor.get_external_tilesets();

        for (const auto& [first_tiled_id, external_tileset] : external_tilesets) {
          const auto save_tileset_result = save_json(external_tileset.path,
                                                     external_tileset.json,
                                                     options.use_indentation ? 2 : 0);
          if (!save_tileset_result.has_value()) {
            runtime::log(LogLevel::kError, "Could not save external tileset");
            return std::unexpected {save_tileset_result.error()};
          }
        }

        return {};
      });
}

}  // namespace tactile
