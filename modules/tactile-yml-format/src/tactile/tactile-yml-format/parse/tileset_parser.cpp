// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/tileset_parser.hpp"

#include <filesystem>  // exists
#include <utility>     // move

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"
#include "tactile/tactile-yml-format/parse/meta_parser.hpp"
#include "tactile/tactile-yml-format/parse/object_parser.hpp"

namespace tactile::yml_format {

auto parse_tileset_tile_animation_frame(const YAML::Node& frame_node)
    -> Result<ir::AnimationFrame>
{
  TACTILE_LOG_TRACE("Parsing tile animation frame node at {}:{}",
                    frame_node.Mark().line,
                    frame_node.Mark().column);

  ir::AnimationFrame frame {};

  return parse_to(frame_node, "tile", frame.tile_index.value)
      .and_then([&] { return parse<Milliseconds::rep>(frame_node, "duration"); })
      .and_then([&](const Milliseconds::rep duration) {
        frame.duration = Milliseconds {duration};
        return kOK;
      })
      .transform([&] { return frame; });
}

auto parse_tileset_tile_animation(const YAML::Node& tile_node)
    -> Result<ir::TileAnimation>
{
  ir::TileAnimation animation {};

  if (const auto frame_seq = tile_node["animation"]) {
    animation.reserve(frame_seq.size());

    for (const auto frame_node : frame_seq) {
      if (auto frame = parse_tileset_tile_animation_frame(frame_node)) {
        animation.push_back(*frame);
      }
      else {
        return propagate_unexpected(frame);
      }
    }
  }

  return animation;
}

auto parse_tileset_tile(const YAML::Node& tile_node, const ir::Map& map)
    -> Result<ir::Tile>
{
  TACTILE_LOG_TRACE("Parsing tile node at {}:{}",
                    tile_node.Mark().line,
                    tile_node.Mark().column);

  ir::Tile tile {};

  return parse_to(tile_node, "id", tile.index.value)
      .and_then([&] { return parse_tileset_tile_animation(tile_node); })
      .and_then([&](ir::TileAnimation&& tile_animation) {
        tile.animation = std::move(tile_animation);
        return kOK;
      })
      .and_then([&] { return parse_objects(tile_node, map); })
      .and_then([&](Vector<ir::Object>&& objects) {
        tile.objects = std::move(objects);
        return kOK;
      })
      .and_then([&] { return parse_metadata(tile_node, map, tile.meta); })
      .transform([&] { return std::move(tile); });
}

auto parse_tileset_tiles(const YAML::Node& tileset_node, const ir::Map& map)
    -> Result<Vector<ir::Tile>>
{
  Vector<ir::Tile> tiles {};

  if (const auto tile_seq = tileset_node["tiles"]) {
    tiles.reserve(tile_seq.size());

    for (const auto tile_node : tile_seq) {
      if (auto tile = parse_tileset_tile(tile_node, map)) {
        tiles.push_back(std::move(*tile));
      }
      else {
        return propagate_unexpected(tile);
      }
    }
  }

  return tiles;
}

auto parse_tileset(const YAML::Node& tileset_node,
                   const ir::Map& map,
                   const SaveFormatReadOptions& options) -> Result<ir::Tileset>
{
  ir::Tileset tileset {};

  return parse<int>(tileset_node, "version")
      .and_then([&](const int version) -> Result<void> {
        if (version != 1) {
          return unexpected(make_save_format_error(SaveFormatError::kUnsupportedVersion));
        }
        return kOK;
      })
      // clang-format off
      .and_then([&] { return parse_to(tileset_node, "name", tileset.name); })
      .and_then([&] { return parse_to(tileset_node, "tile-count", tileset.tile_count); })
      .and_then([&] { return parse_to(tileset_node, "tile-width", tileset.tile_width); })
      .and_then([&] { return parse_to(tileset_node, "tile-height", tileset.tile_height); })
      .and_then([&] { return parse_to(tileset_node, "column-count", tileset.column_count); })
      .and_then([&] { return parse_to(tileset_node, "image-width", tileset.image_width); })
      .and_then([&] { return parse_to(tileset_node, "image-height", tileset.image_height); })
      // clang-format on
      .and_then([&] { return parse<String>(tileset_node, "image-path"); })
      .and_then([&](const String& image_filename) -> Result<void> {
        tileset.image_path = image_filename;

        if (!std::filesystem::exists(options.base_dir / tileset.image_path)) {
          TACTILE_LOG_WARN("Tileset references non-existent image {}", image_filename);
          if (options.strict_mode) {
            return unexpected(make_generic_error(GenericError::kNoSuchFile));
          }
        }

        return kOK;
      })
      .and_then([&] { return parse_tileset_tiles(tileset_node, map); })
      .and_then([&](Vector<ir::Tile>&& tiles) {
        tileset.tiles = std::move(tiles);
        return kOK;
      })
      .and_then([&] { return parse_metadata(tileset_node, map, tileset.meta); })
      .transform([&] { return std::move(tileset); });
}

auto parse_tileset_ref(const YAML::Node& tileset_ref_node,
                       const ir::Map& map,
                       const SaveFormatReadOptions& options) -> Result<ir::TilesetRef>
{
  TACTILE_LOG_TRACE("Parsing tileset reference node at {}:{}",
                    tileset_ref_node.Mark().line,
                    tileset_ref_node.Mark().column);

  ir::TilesetRef tileset_ref {};

  return parse_to(tileset_ref_node, "first-global-id", tileset_ref.first_tile_id.value)
      .and_then([&] { return parse<String>(tileset_ref_node, "path"); })
      .and_then([&](const String& tileset_filename) -> Result<YAML::Node> {
        const auto tileset_path = options.base_dir / tileset_filename;
        return parse_yaml_file(tileset_path);
      })
      .and_then([&](const YAML::Node& tileset_node) {
        return parse_tileset(tileset_node, map, options);
      })
      .and_then([&](ir::Tileset&& tileset) {
        tileset_ref.tileset = std::move(tileset);
        return kOK;
      })
      .transform([&] { return std::move(tileset_ref); });
}

auto parse_tileset_refs(const YAML::Node& root_node,
                        const ir::Map& map,
                        const SaveFormatReadOptions& options)
    -> Result<Vector<ir::TilesetRef>>
{
  Vector<ir::TilesetRef> tileset_refs {};

  if (const auto tileset_ref_seq = root_node["tilesets"]) {
    tileset_refs.reserve(tileset_ref_seq.size());

    for (const auto tileset_ref_node : tileset_ref_seq) {
      if (auto tileset_ref = parse_tileset_ref(tileset_ref_node, map, options)) {
        tileset_refs.push_back(std::move(*tileset_ref));
      }
      else {
        return propagate_unexpected(tileset_ref);
      }
    }
  }

  return tileset_refs;
}

}  // namespace tactile::yml_format
