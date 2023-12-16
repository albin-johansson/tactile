// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/emit/tileset_emitter.hpp"

#include <filesystem>  // relative
#include <utility>     // move

#include <fmt/format.h>

#include "tactile/foundation/io/stream.hpp"
#include "tactile/tiled-json-format/emit/meta_emitter.hpp"
#include "tactile/tiled-json-format/emit/tile_emitter.hpp"

namespace tactile::tiled::tmj {
namespace {

[[nodiscard]]
auto _add_common_tileset_attributes(const ir::Tileset& tileset,
                                    const SaveFormatWriteOptions& options,
                                    JSON& tileset_json) -> Result<void>
{
  tileset_json["type"] = "tileset";
  tileset_json["name"] = tileset.name;

  tileset_json["columns"] = tileset.column_count;
  tileset_json["tilewidth"] = tileset.tile_width;
  tileset_json["tileheight"] = tileset.tile_height;
  tileset_json["tilecount"] = tileset.tile_count;

  tileset_json["image"] = normalize_path(tileset.image_path);
  tileset_json["imagewidth"] = tileset.image_width;
  tileset_json["imageheight"] = tileset.image_height;

  tileset_json["margin"] = 0;
  tileset_json["spacing"] = 0;

  if (options.use_external_tilesets) {
    tileset_json["tiledversion"] = "1.9.2";
    tileset_json["version"] = "1.7";
  }

  if (!tileset.meta.properties.empty()) {
    tileset_json["properties"] = emit_property_array(tileset.meta);
  }

  if (!tileset.tiles.empty()) {
    tileset_json["tiles"] = emit_tile_definition_array(tileset.tiles);
  }

  return kSuccess;
}

[[nodiscard]]
auto _determine_external_tileset_filename(const ir::TilesetRef& tileset_ref) -> String
{
  return !tileset_ref.tileset.name.empty()
             ? fmt::format("{}.tmj", tileset_ref.tileset.name)
             : fmt::format("tileset_{}.tmj", tileset_ref.first_tile_id.value);
}

}  // namespace

auto emit_embedded_tileset(const ir::Tileset& tileset,
                           const TileID first_tile_id,
                           const SaveFormatWriteOptions& options) -> Result<JSON>
{
  auto embedded_tileset_json = JSON::object();
  embedded_tileset_json["firstgid"] = first_tile_id.value;

  return _add_common_tileset_attributes(tileset, options, embedded_tileset_json)
      .and_then([&]() -> Result<JSON> { return std::move(embedded_tileset_json); });
}

auto emit_external_tileset(const ir::TilesetRef& tileset_ref,
                           const SaveFormatWriteOptions& options) -> Result<JSON>
{
  auto external_tileset_json = JSON::object();

  external_tileset_json["firstgid"] = tileset_ref.first_tile_id.value;
  external_tileset_json["source"] = _determine_external_tileset_filename(tileset_ref);

  return external_tileset_json;
}

auto emit_tileset_ref(const ir::TilesetRef& tileset_ref,
                      const SaveFormatWriteOptions& options) -> Result<JSON>
{
  if (options.use_external_tilesets) {
    const auto external_tileset_filename =
        _determine_external_tileset_filename(tileset_ref);
    const auto external_tileset_path = options.base_dir / external_tileset_filename;

    auto external_tileset_json = JSON::object();
    return _add_common_tileset_attributes(tileset_ref.tileset,
                                          options,
                                          external_tileset_json)
        .and_then([&] {
          const StreamToFileOptions stream_options {
            .indentation = options.use_indentation ? 2 : 0,
            .binary_mode = false,
          };

          stream_to_file(external_tileset_json, external_tileset_path, stream_options);
          return kSuccess;
        })
        .and_then([&] { return emit_external_tileset(tileset_ref, options); });
  }
  else {
    return emit_embedded_tileset(tileset_ref.tileset, tileset_ref.first_tile_id, options);
  }
}

auto emit_tileset_array(const ir::Map& map, const SaveFormatWriteOptions& options)
    -> Result<JSON>
{
  auto tileset_json_array = JSON::array();

  for (const auto& tileset_ref : map.tilesets) {
    if (auto tileset_json = emit_tileset_ref(tileset_ref, options)) {
      tileset_json_array += std::move(*tileset_json);
    }
    else {
      return propagate_unexpected(tileset_json);
    }
  }

  return tileset_json_array;
}

}  // namespace tactile::tiled::tmj
