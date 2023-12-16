// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/parse/tileset_parser.hpp"

#include <filesystem>  // exists
#include <iterator>    // distance
#include <utility>     // move

#include <fmt/ostream.h>

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/misc/conversion.hpp"
#include "tactile/tiled-xml-format/common.hpp"
#include "tactile/tiled-xml-format/parse/meta_parser.hpp"
#include "tactile/tiled-xml-format/parse/object_parser.hpp"

namespace tactile::tiled::tmx {

auto parse_tileset_tile_animation_frame_node(const pugi::xml_node animation_frame_node)
    -> Result<ir::AnimationFrame>
{
  ir::AnimationFrame frame {};
  return parse_to(animation_frame_node, "tileid", frame.tile_index.value)
      .and_then([&] { return parse_uint(animation_frame_node, "duration"); })
      .and_then([&](const uint duration) {
        frame.duration = Milliseconds {static_cast<Milliseconds::rep>(duration)};
        return kOK;
      })
      .transform([&] { return frame; });
}

auto parse_tileset_tile_animation_node(const pugi::xml_node tile_node)
    -> Result<ir::TileAnimation>
{
  ir::TileAnimation animation {};

  const auto animation_frame_node_range = tile_node.child("animation").children("frame");
  if (animation_frame_node_range.empty()) {
    return animation;
  }

  animation.reserve(as_unsigned(std::distance(animation_frame_node_range.begin(),
                                              animation_frame_node_range.end())));

  for (const auto animation_frame_node : animation_frame_node_range) {
    if (const auto animation_frame =
            parse_tileset_tile_animation_frame_node(animation_frame_node)) {
      animation.push_back(*animation_frame);
    }
    else {
      return propagate_unexpected(animation_frame);
    }
  }

  return animation;
}

auto parse_tileset_tile_node(const pugi::xml_node tile_node) -> Result<ir::Tile>
{
  ir::Tile tile {};

  return parse_to(tile_node, "id", tile.index.value)
      .and_then([&] { return parse_metadata(tile_node, tile.meta); })
      .and_then([&] { return parse_tileset_tile_animation_node(tile_node); })
      .and_then([&](ir::TileAnimation&& tile_animation) {
        tile.animation = std::move(tile_animation);
        return kOK;
      })
      .and_then([&] { return parse_objects(tile_node.child("objectgroup")); })
      .and_then([&](Vector<ir::Object>&& objects) {
        tile.objects = std::move(objects);
        return kOK;
      })
      .transform([&] { return std::move(tile); });
}

auto parse_tileset_tiles(const pugi::xml_node tileset_node) -> Result<Vector<ir::Tile>>
{
  Vector<ir::Tile> tiles {};

  const auto tile_node_range = tileset_node.children("tile");
  if (tile_node_range.empty()) {
    return tiles;
  }

  tiles.reserve(
      as_unsigned(std::distance(tile_node_range.begin(), tile_node_range.end())));

  for (const auto tile_node : tile_node_range) {
    if (auto tile = parse_tileset_tile_node(tile_node)) {
      tiles.push_back(std::move(*tile));
    }
    else {
      return propagate_unexpected(tile);
    }
  }

  return tiles;
}

auto parse_tileset_image_node(const pugi::xml_node tileset_node,
                              ir::Tileset& tileset,
                              const SaveFormatReadOptions& options) -> Result<void>
{
  const auto image_node = tileset_node.child("image");

  return parse_to(image_node, "width", tileset.image_width)
      .and_then([&] { return parse_to(image_node, "height", tileset.image_height); })
      .and_then([&] { return parse_string(image_node, "source"); })
      .and_then([&](const String& image_filename) -> Result<void> {
        tileset.image_path = image_filename;

        if (!std::filesystem::exists(options.base_dir / tileset.image_path)) {
          TACTILE_LOG_WARN("Tileset references non-existent image {}", image_filename);
          if (options.strict_mode) {
            return unexpected(make_generic_error(GenericError::kNoSuchFile));
          }
        }

        return kOK;
      });
}

auto parse_embedded_tileset_node(const pugi::xml_node tileset_node,
                                 const SaveFormatReadOptions& options)
    -> Result<ir::Tileset>
{
  ir::Tileset tileset {};

  return parse_to(tileset_node, "name", tileset.name)
      .and_then([&] { return parse_to(tileset_node, "tilewidth", tileset.tile_width); })
      .and_then([&] { return parse_to(tileset_node, "tileheight", tileset.tile_height); })
      .and_then([&] { return parse_to(tileset_node, "tilecount", tileset.tile_count); })
      .and_then([&] { return parse_to(tileset_node, "columns", tileset.column_count); })
      .and_then([&] { return parse_tileset_image_node(tileset_node, tileset, options); })
      .and_then([&] { return parse_tileset_tiles(tileset_node); })
      .and_then([&](Vector<ir::Tile>&& tiles) {
        tileset.tiles = std::move(tiles);
        return kOK;
      })
      .and_then([&] { return parse_metadata(tileset_node, tileset.meta); })
      .transform([&] { return std::move(tileset); });
}

auto parse_external_tileset(const StringView tileset_filename,
                            const SaveFormatReadOptions& options) -> Result<ir::Tileset>
{
  const auto tileset_path = options.base_dir / tileset_filename;
  TACTILE_LOG_DEBUG("Parsing external tileset from {}", fmt::streamed(tileset_path));

  return load_xml_file(tileset_path).and_then([&](const pugi::xml_document& document) {
    const auto tileset_node = document.root().child("tileset");
    return parse_embedded_tileset_node(tileset_node, options);
  });
}

auto parse_tileset_node(const pugi::xml_node tileset_node,
                        const SaveFormatReadOptions& options) -> Result<ir::TilesetRef>
{
  ir::TilesetRef tileset_ref {};

  return parse_to(tileset_node, "firstgid", tileset_ref.first_tile_id.value)
      .and_then([&] {
        if (!tileset_node.attribute("source").empty()) {
          return parse_string(tileset_node, "source")
              .and_then([&](const String& tileset_filename) {
                return parse_external_tileset(tileset_filename, options);
              });
        }
        else {
          return parse_embedded_tileset_node(tileset_node, options);
        }
      })
      .and_then([&](ir::Tileset&& tileset) {
        tileset_ref.tileset = std::move(tileset);
        return kOK;
      })
      .transform([&] { return std::move(tileset_ref); });
}

auto parse_tilesets(const pugi::xml_node map_node, const SaveFormatReadOptions& options)
    -> Result<Vector<ir::TilesetRef>>
{
  Vector<ir::TilesetRef> tilesets {};

  const auto tileset_node_range = map_node.children("tileset");
  if (tileset_node_range.empty()) {
    return tilesets;
  }

  tilesets.reserve(
      as_unsigned(std::distance(tileset_node_range.begin(), tileset_node_range.end())));

  for (const auto tileset_node : tileset_node_range) {
    if (auto tileset = parse_tileset_node(tileset_node, options)) {
      tilesets.push_back(std::move(*tileset));
    }
    else {
      return propagate_unexpected(tileset);
    }
  }

  return tilesets;
}

}  // namespace tactile::tiled::tmx
