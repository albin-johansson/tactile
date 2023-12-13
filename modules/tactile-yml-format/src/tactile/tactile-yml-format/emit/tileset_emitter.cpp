// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/tileset_emitter.hpp"

#include <fmt/format.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/stream.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/emit/meta_emitter.hpp"
#include "tactile/tactile-yml-format/emit/object_emitter.hpp"

namespace tactile::yml_format {
namespace {

// TODO move to IR?
[[nodiscard]]
auto _determine_external_tileset_filename(const ir::TilesetRef& tileset_ref) -> String
{
  return !tileset_ref.tileset.meta.name.empty()
             ? fmt::format("{}.yml", tileset_ref.tileset.meta.name)
             : fmt::format("tileset_{}.yml", tileset_ref.first_tile_id.value);
}

}  // namespace

void emit_tileset_tile_animation(YAML::Emitter& emitter, const ir::Tile& tile)
{
  if (tile.animation.empty()) {
    return;
  }

  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  for (const auto& frame : tile.animation) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "tile" << YAML::Value << frame.tile_index.value;
    emitter << YAML::Key << "duration" << YAML::Value << frame.duration.count();
    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void emit_tileset_tile(YAML::Emitter& emitter, const ir::Tile& tile)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << tile.index.value;

  emit_tileset_tile_animation(emitter, tile);
  emit_object_seq(emitter, tile.objects);
  emit_metadata(emitter, tile.meta);

  emitter << YAML::EndMap;
}

void emit_tileset_tile_seq(YAML::Emitter& emitter, const ir::Tileset& tileset)
{
  if (tileset.tiles.empty()) {
    return;
  }

  emitter << YAML::Key << "tiles" << YAML::BeginSeq;

  for (const auto& tile : tileset.tiles) {
    emit_tileset_tile(emitter, tile);
  }

  emitter << YAML::EndSeq;
}

void emit_tileset(YAML::Emitter& emitter,
                  const ir::Tileset& tileset,
                  const SaveFormatWriteOptions& options)
{
  emitter << YAML::BeginMap;

  emitter << YAML::Key << "version" << YAML::Value << 1;
  emitter << YAML::Key << "name" << YAML::Value << tileset.meta.name;

  emitter << YAML::Key << "tile-width" << YAML::Value << tileset.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << tileset.tile_height;

  emitter << YAML::Key << "tile-count" << YAML::Value << tileset.tile_count;
  emitter << YAML::Key << "column-count" << YAML::Value << tileset.column_count;

  const auto image_path = std::filesystem::relative(tileset.image_path, options.base_dir);
  emitter << YAML::Key << "image-path" << YAML::Value << normalize_path(image_path);
  emitter << YAML::Key << "image-width" << YAML::Value << tileset.image_width;
  emitter << YAML::Key << "image-height" << YAML::Value << tileset.image_height;

  emit_tileset_tile_seq(emitter, tileset);
  emit_metadata(emitter, tileset.meta);

  emitter << YAML::EndMap;
}

void emit_tileset_ref(YAML::Emitter& emitter,
                      const ir::TilesetRef& tileset_ref,
                      const SaveFormatWriteOptions& options)
{
  const auto external_tileset_filename =
      _determine_external_tileset_filename(tileset_ref);
  const auto external_tileset_path = options.base_dir / external_tileset_filename;

  TACTILE_LOG_DEBUG("Saving external tileset to {}", external_tileset_filename);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "first-global-id" << YAML::Value
          << tileset_ref.first_tile_id.value;
  emitter << YAML::Key << "path" << YAML::Value << external_tileset_filename;
  emitter << YAML::EndMap;

  YAML::Emitter tileset_emitter {};
  tileset_emitter.SetIndent(2);

  emit_tileset(tileset_emitter, tileset_ref.tileset, options);

  stream_to_file(tileset_emitter.c_str(),
                 external_tileset_path,
                 StreamToFileOptions {.indentation = 0, .binary_mode = false})
      .or_else([](const std::error_code& error_code) {
        TACTILE_LOG_ERROR("Could not emit external tileset: {}", error_code.message());
      });
}

void emit_tileset_seq(YAML::Emitter& emitter,
                      const ir::Map& map,
                      const SaveFormatWriteOptions& options)
{
  if (map.tilesets.empty()) {
    return;
  }

  emitter << YAML::Key << "tilesets" << YAML::BeginSeq;

  for (const auto& tileset_ref : map.tilesets) {
    emit_tileset_ref(emitter, tileset_ref, options);
  }

  emitter << YAML::EndSeq;
}

}  // namespace tactile::yml_format
