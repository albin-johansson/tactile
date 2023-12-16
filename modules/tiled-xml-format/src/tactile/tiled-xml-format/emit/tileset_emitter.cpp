// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/emit/tileset_emitter.hpp"

#include <filesystem>  // relative

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tiled-xml-format/common.hpp"
#include "tactile/tiled-xml-format/emit/object_emitter.hpp"
#include "tactile/tiled-xml-format/emit/property_emitter.hpp"
#include "tactile/tiled-xml-format/version.hpp"

namespace tactile::tiled::tmx {
namespace {

// TODO move to IR?
[[nodiscard]]
auto _determine_external_tileset_filename(const ir::TilesetRef& tileset_ref) -> String
{
  return !tileset_ref.tileset.meta.name.empty()
             ? fmt::format("{}.tmx", tileset_ref.tileset.meta.name)
             : fmt::format("tileset_{}.tmx", tileset_ref.first_tile_id.value);
}

void _append_tileset_image_node(pugi::xml_node tileset_node, const ir::Tileset& tileset)
{
  auto image_node = tileset_node.append_child("image");

  const auto image_path = normalize_path(tileset.image_path);
  image_node.append_attribute("source").set_value(image_path.c_str());

  image_node.append_attribute("width").set_value(tileset.image_width);
  image_node.append_attribute("height").set_value(tileset.image_height);
}

void _append_tile_node(pugi::xml_node tileset_node, const ir::Tile& tile)
{
  auto tile_node = tileset_node.append_child("tile");
  tile_node.append_attribute("id").set_value(tile.index.value);

  if (!tile.animation.empty()) {
    auto animation_node = tile_node.append_child("animation");

    for (const auto& frame : tile.animation) {
      auto frame_node = animation_node.append_child("frame");
      frame_node.append_attribute("tileid").set_value(frame.tile_index.value);
      frame_node.append_attribute("duration").set_value(frame.duration.count());
    }
  }

  if (!tile.objects.empty()) {
    auto object_layer_node = tile_node.append_child("objectgroup");
    object_layer_node.append_attribute("draworder").set_value("index");

    for (const auto& object : tile.objects) {
      append_object_node(object_layer_node, object);
    }
  }

  append_properties_node(tile_node, tile.meta.properties);
}

void _append_common_tileset_attributes(pugi::xml_node tileset_node,
                                       const ir::Tileset& tileset)
{
  tileset_node.append_attribute("name").set_value(tileset.meta.name.c_str());
  tileset_node.append_attribute("tilewidth").set_value(tileset.tile_width);
  tileset_node.append_attribute("tileheight").set_value(tileset.tile_height);
  tileset_node.append_attribute("tilecount").set_value(tileset.tile_count);
  tileset_node.append_attribute("columns").set_value(tileset.column_count);

  _append_tileset_image_node(tileset_node, tileset);

  for (const auto& tile : tileset.tiles) {
    _append_tile_node(tileset_node, tile);
  }

  append_properties_node(tileset_node, tileset.meta.properties);
}

}  // namespace

auto emit_external_tileset_file(const FilePath& tileset_path,
                                const ir::Tileset& tileset,
                                const SaveFormatWriteOptions& options) -> Result<void>
{
  pugi::xml_document document {};

  auto tileset_node = document.append_child("tileset");
  tileset_node.append_attribute("version").set_value(kTiledFormatVersion);
  tileset_node.append_attribute("tiledversion").set_value(kTiledVersion);

  _append_common_tileset_attributes(tileset_node, tileset);

  const auto save_result =
      save_xml_file(document, tileset_path, options.use_indentation ? 2 : 0);

  if (!save_result.has_value()) {
    TACTILE_LOG_ERROR("Could not save tileset file: {}", save_result.error().message());
  }

  return save_result;
}

void append_external_tileset_node(pugi::xml_node root_node,
                                  const ir::TilesetRef& tileset_ref,
                                  const FilePath& tileset_path)
{
  auto tileset_node = root_node.append_child("tileset");
  tileset_node.append_attribute("firstgid").set_value(tileset_ref.first_tile_id.value);
  tileset_node.append_attribute("source").set_value(tileset_path.string().c_str());
}

void append_embedded_tileset_node(pugi::xml_node root_node,
                                  const ir::TilesetRef& tileset_ref)
{
  auto tileset_node = root_node.append_child("tileset");
  tileset_node.append_attribute("firstgid").set_value(tileset_ref.first_tile_id.value);

  _append_common_tileset_attributes(tileset_node, tileset_ref.tileset);
}

void append_tileset_node(pugi::xml_node root_node,
                         const ir::TilesetRef& tileset_ref,
                         const SaveFormatWriteOptions& options)
{
  if (options.use_external_tilesets) {
    const auto tileset_filename = _determine_external_tileset_filename(tileset_ref);
    const auto tileset_path = options.base_dir / tileset_filename;
    TACTILE_LOG_TRACE("Saving external tileset to {}", fmt::streamed(tileset_path));

    append_external_tileset_node(root_node, tileset_ref, tileset_filename);
    emit_external_tileset_file(tileset_path, tileset_ref.tileset, options);
  }
  else {
    append_embedded_tileset_node(root_node, tileset_ref);
  }
}

}  // namespace tactile::tiled::tmx
