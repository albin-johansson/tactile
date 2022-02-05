#include "xml_tilesets.hpp"

#include <filesystem>  // weakly_canonical, exists
#include <utility>     // move

#include "assert.hpp"
#include "io/maps/xml_utils.hpp"
#include "xml_attributes.hpp"
#include "xml_layers.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_fancy_tiles(pugi::xml_node tilesetNode,
                                      ir::tileset_data& tilesetData) -> parse_error
{
  for (auto tileNode : tilesetNode.children("tile")) {
    tile_id tileId{};

    if (const auto id = int_attribute(tileNode, "id")) {
      tileId = *id;
    }
    else {
      return parse_error::no_fancy_tile_id;
    }

    auto& tileData = tilesetData.fancy_tiles[tileId];

    for (auto frameNode : tileNode.child("animation").children("frame")) {
      auto& frameData = tileData.frames.emplace_back();

      if (const auto localId = int_attribute(frameNode, "tileid")) {
        frameData.local_id = *localId;
      }
      else {
        return parse_error::no_animation_frame_tile;
      }

      if (const auto duration = uint_attribute(frameNode, "duration")) {
        frameData.duration_ms = *duration;
      }
      else {
        return parse_error::no_animation_frame_duration;
      }
    }

    for (auto objectNode : tileNode.child("objectgroup").children("object")) {
      auto& objectData = tileData.objects.emplace_back();
      if (const auto err = parse_object(objectNode, objectData);
          err != parse_error::none) {
        return err;
      }
    }

    if (const auto err = parse_properties(tileNode, tileData.context);
        err != parse_error::none) {
      return err;
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_image_info(pugi::xml_node tilesetNode,
                                     ir::tileset_data& tilesetData,
                                     const std::filesystem::path& dir) -> parse_error
{
  auto imageNode = tilesetNode.child("image");

  if (const auto width = int_attribute(imageNode, "width")) {
    tilesetData.image_width = *width;
  }
  else {
    return parse_error::no_tileset_image_width;
  }

  if (const auto height = int_attribute(imageNode, "height")) {
    tilesetData.image_height = *height;
  }
  else {
    return parse_error::no_tileset_image_height;
  }

  const auto relativePath = string_attribute(imageNode, "source");
  if (!relativePath) {
    return parse_error::no_tileset_image_path;
  }

  auto absolutePath = std::filesystem::weakly_canonical(dir / *relativePath);
  if (std::filesystem::exists(absolutePath)) {
    tilesetData.image_path = std::move(absolutePath);
  }
  else {
    return parse_error::tileset_image_does_not_exist;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_common_attributes(pugi::xml_node node,
                                            ir::tileset_data& tilesetData,
                                            const std::filesystem::path& dir)
    -> parse_error
{
  if (auto name = string_attribute(node, "name")) {
    tilesetData.name = std::move(*name);
  }
  else {
    return parse_error::no_tileset_name;
  }

  if (const auto tw = int_attribute(node, "tilewidth")) {
    tilesetData.tile_width = *tw;
  }
  else {
    return parse_error::no_tileset_tile_width;
  }

  if (const auto th = int_attribute(node, "tileheight")) {
    tilesetData.tile_height = *th;
  }
  else {
    return parse_error::no_tileset_tile_height;
  }

  if (const auto count = int_attribute(node, "tilecount")) {
    tilesetData.tile_count = *count;
  }
  else {
    return parse_error::no_tileset_tile_count;
  }

  if (const auto columns = int_attribute(node, "columns")) {
    tilesetData.column_count = *columns;
  }
  else {
    return parse_error::no_tileset_column_count;
  }

  if (const auto err = _parse_image_info(node, tilesetData, dir);
      err != parse_error::none) {
    return err;
  }

  if (const auto err = _parse_fancy_tiles(node, tilesetData); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_properties(node, tilesetData.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_external_tileset(pugi::xml_node node,
                                           ir::tileset_data& tilesetData,
                                           const std::filesystem::path& dir)
    -> parse_error
{
  TACTILE_ASSERT(has_attribute(node, "source"));

  const auto relativePath = string_attribute(node, "source").value();
  const auto sourcePath = std::filesystem::weakly_canonical(dir / relativePath);

  if (!std::filesystem::exists(sourcePath)) {
    return parse_error::external_tileset_does_not_exist;
  }

  pugi::xml_document document;
  if (!document.load_file(sourcePath.c_str())) {
    return parse_error::unknown_external_tileset_error;
  }

  return _parse_common_attributes(document.child("tileset"), tilesetData, dir);
}

}  // namespace

auto parse_tileset(pugi::xml_node node,
                   ir::tileset_data& tilesetData,
                   const std::filesystem::path& dir) -> parse_error
{
  if (const auto firstTile = int_attribute(node, "firstgid")) {
    tilesetData.first_tile = *firstTile;
  }
  else {
    return parse_error::no_tileset_first_tile_id;
  }

  if (has_attribute(node, "source")) {
    return _parse_external_tileset(node, tilesetData, dir);
  }
  else {
    return _parse_common_attributes(node, tilesetData, dir);
  }
}

}  // namespace tactile::parsing