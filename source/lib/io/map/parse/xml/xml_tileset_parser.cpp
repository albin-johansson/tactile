// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include "io/ir/map/map_ir.hpp"
#include "io/map/parse/xml/xml_parser.hpp"
#include "io/util/xml.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto parse_fancy_tiles(XmlNode tileset_node)
    -> Expected<TilesetIR::MetaTiles, ParseError>
{
  TilesetIR::MetaTiles tiles;

  for (const auto tile_node: tileset_node.children("tile")) {
    TileID tile_id {};

    if (const auto id = get_int_attr(tile_node, "id")) {
      tile_id = *id;
    }
    else {
      return unexpected(ParseError::NoFancyTileId);
    }

    auto& tile = tiles[tile_id];

    for (const auto frame_node: tile_node.child("animation").children("frame")) {
      auto& frame = tile.frames.emplace_back();

      if (const auto index = get_int_attr(frame_node, "tileid")) {
        frame.tile_index = *index;
      }
      else {
        return unexpected(ParseError::NoAnimationFrameTile);
      }

      if (const auto duration = get_uint_attr(frame_node, "duration")) {
        frame.duration_ms = *duration;
      }
      else {
        return unexpected(ParseError::NoAnimationFrameDuration);
      }
    }

    for (const auto object_node: tile_node.child("objectgroup").children("object")) {
      if (auto object = parse_object(object_node)) {
        tile.objects.push_back(std::move(*object));
      }
      else {
        return propagate_unexpected(object);
      }
    }

    if (auto props = parse_properties(tile_node)) {
      tile.context.properties = std::move(*props);
    }
    else {
      return propagate_unexpected(props);
    }
  }

  return tiles;
}

[[nodiscard]] auto parse_image_info(XmlNode tileset_node,
                                    TilesetIR& tileset,
                                    const Path& dir) -> ParseError
{
  const auto image_node = tileset_node.child("image");

  if (const auto width = get_int_attr(image_node, "width")) {
    tileset.image_size[0] = *width;
  }
  else {
    return ParseError::NoTilesetImageWidth;
  }

  if (const auto height = get_int_attr(image_node, "height")) {
    tileset.image_size[1] = *height;
  }
  else {
    return ParseError::NoTilesetImageHeight;
  }

  const auto relative_path = get_string_attr(image_node, "source");
  if (!relative_path) {
    return ParseError::NoTilesetImagePath;
  }

  auto absolute_path = fs::weakly_canonical(dir / *relative_path);
  if (fs::exists(absolute_path)) {
    tileset.image_path = std::move(absolute_path);
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  return ParseError::None;
}

[[nodiscard]] auto parse_common_attributes(XmlNode node,
                                           const TileID first_id,
                                           const Path& dir)
    -> Expected<TilesetIR, ParseError>
{
  TilesetIR tileset;
  tileset.first_tile = first_id;

  if (auto name = get_string_attr(node, "name")) {
    tileset.name = std::move(*name);
  }
  else {
    return unexpected(ParseError::NoTilesetName);
  }

  if (const auto tw = get_int_attr(node, "tilewidth")) {
    tileset.tile_size[0] = *tw;
  }
  else {
    return unexpected(ParseError::NoTilesetTileWidth);
  }

  if (const auto th = get_int_attr(node, "tileheight")) {
    tileset.tile_size[1] = *th;
  }
  else {
    return unexpected(ParseError::NoTilesetTileHeight);
  }

  if (const auto count = get_int_attr(node, "tilecount")) {
    tileset.tile_count = *count;
  }
  else {
    return unexpected(ParseError::NoTilesetTileCount);
  }

  if (const auto columns = get_int_attr(node, "columns")) {
    tileset.column_count = *columns;
  }
  else {
    return unexpected(ParseError::NoTilesetColumnCount);
  }

  if (const auto ec = parse_image_info(node, tileset, dir); ec != ParseError::None) {
    return unexpected(ec);
  }

  if (auto tiles = parse_fancy_tiles(node)) {
    tileset.fancy_tiles = std::move(*tiles);
  }
  else {
    return propagate_unexpected(tiles);
  }

  if (auto props = parse_properties(node)) {
    tileset.context.properties = std::move(*props);
  }
  else {
    return propagate_unexpected(props);
  }

  return tileset;
}

[[nodiscard]] auto parse_external_tileset(XmlNode node,
                                          const TileID first_id,
                                          const Path& dir)
    -> Expected<TilesetIR, ParseError>
{
  TACTILE_ASSERT(has_attr(node, "source"));

  const auto relative_path = get_string_attr(node, "source").value();
  const auto source_path = fs::weakly_canonical(dir / relative_path);

  if (!fs::exists(source_path)) {
    return unexpected(ParseError::ExternalTilesetDoesNotExist);
  }

  pugi::xml_document document;
  if (!document.load_file(source_path.c_str(),
                          pugi::parse_default | pugi::parse_trim_pcdata)) {
    return unexpected(ParseError::UnknownExternalTilesetError);
  }

  return parse_common_attributes(document.child("tileset"), first_id, dir);
}

}  // namespace

auto parse_tileset(XmlNode node, const Path& dir) -> Expected<TilesetIR, ParseError>
{
  TileID first_id {};
  if (const auto id = get_int_attr(node, "firstgid")) {
    first_id = *id;
  }
  else {
    return unexpected(ParseError::NoTilesetFirstTileId);
  }

  if (has_attr(node, "source")) {
    return parse_external_tileset(node, first_id, dir);
  }
  else {
    return parse_common_attributes(node, first_id, dir);
  }
}

}  // namespace tactile
