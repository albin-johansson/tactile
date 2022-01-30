#include "yaml_tilesets.hpp"

#include <filesystem>  // exists, weakly_canonical
#include <string>      // string
#include <utility>     // move

#include "yaml_attributes.hpp"
#include "yaml_layers.hpp"

namespace tactile::parsing {
namespace {

constexpr int32 _tileset_file_version = 1;

[[nodiscard]] auto _parse_animation_frame(const YAML::Node& node,
                                          ir::fancy_tile_data& tile) -> parse_error
{
  tile_id frameTile{};
  uint32 frameDuration{};

  if (auto id = node["tile"]) {
    frameTile = id.as<tile_id>();
  }
  else {
    return parse_error::no_animation_frame_tile;
  }

  if (auto duration = node["duration"]) {
    frameDuration = duration.as<uint32>();
  }
  else {
    return parse_error::no_animation_frame_duration;
  }

  auto& frame = tile.frames.emplace_back();
  frame.local_id = frameTile;
  frame.duration_ms = frameDuration;

  return parse_error::none;
}

[[nodiscard]] auto _parse_fancy_tile(const YAML::Node& node,
                                     const ir::map_data& map,
                                     ir::tileset_data& tileset) -> parse_error
{
  tile_id tileId{};

  if (auto id = node["id"]) {
    tileId = id.as<tile_id>();
  }
  else {
    return parse_error::no_fancy_tile_id;
  }

  auto& tile = tileset.fancy_tiles[tileId];

  if (auto sequence = node["animation"]) {
    tile.frames.reserve(sequence.size());

    for (const auto& frame : sequence) {
      if (const auto err = _parse_animation_frame(frame, tile);
          err != parse_error::none) {
        return err;
      }
    }
  }

  if (auto sequence = node["objects"]) {
    tile.objects.reserve(sequence.size());

    for (const auto& objectNode : sequence) {
      auto& object = tile.objects.emplace_back();
      if (const auto err = parse_object(objectNode, map, &object);
          err != parse_error::none) {
        return err;
      }
    }
  }

  if (const auto err = parse_properties(node, tile.context); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_components(node, map, tile.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_fancy_tiles(const YAML::Node& sequence,
                                      const ir::map_data& map,
                                      ir::tileset_data& tileset) -> parse_error
{
  tileset.fancy_tiles.reserve(sequence.size());

  for (const auto& node : sequence) {
    if (const auto err = _parse_fancy_tile(node, map, tileset);
        err != parse_error::none) {
      return err;
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_tileset(const std::filesystem::path& source,
                                  ir::map_data& map,
                                  const tile_id firstTileId) -> parse_error
{
  try {
    const auto node = YAML::LoadFile(source.string());
    if (!node) {
      return parse_error::could_not_read_external_tileset;
    }

    const auto dir = source.parent_path();

    auto& tileset = map.tilesets.emplace_back();
    tileset.first_tile = firstTileId;

    if (auto version = node["version"]) {
      if (version.as<int32>() != _tileset_file_version) {
        return parse_error::unsupported_tileset_version;
      }
    }
    else {
      return parse_error::no_tileset_version;
    }

    if (auto name = node["name"]) {
      tileset.name = name.as<std::string>();
    }
    else {
      return parse_error::no_tileset_name;
    }

    if (auto count = node["tile-count"]) {
      tileset.tile_count = count.as<int32>();
    }
    else {
      return parse_error::no_tileset_tile_count;
    }

    if (auto tw = node["tile-width"]) {
      tileset.tile_width = tw.as<int32>();
    }
    else {
      return parse_error::no_tileset_tile_width;
    }

    if (auto th = node["tile-height"]) {
      tileset.tile_height = th.as<int32>();
    }
    else {
      return parse_error::no_tileset_tile_height;
    }

    if (auto columns = node["column-count"]) {
      tileset.column_count = columns.as<int32>();
    }
    else {
      return parse_error::no_tileset_column_count;
    }

    if (auto rel = node["image-path"]) {
      auto abs = std::filesystem::weakly_canonical(dir / rel.as<std::string>());
      if (std::filesystem::exists(abs)) {
        tileset.image_path = std::move(abs);
      }
      else {
        return parse_error::tileset_image_does_not_exist;
      }
    }
    else {
      return parse_error::no_tileset_image_path;
    }

    if (auto imageWidth = node["image-width"]) {
      tileset.image_width = imageWidth.as<int32>();
    }
    else {
      return parse_error::no_tileset_image_width;
    }

    if (auto imageHeight = node["image-height"]) {
      tileset.image_height = imageHeight.as<int32>();
    }
    else {
      return parse_error::no_tileset_image_height;
    }

    if (auto sequence = node["tiles"]) {
      if (const auto err = _parse_fancy_tiles(sequence, map, tileset);
          err != parse_error::none) {
        return err;
      }
    }

    if (const auto err = parse_properties(node, tileset.context);
        err != parse_error::none) {
      return err;
    }

    if (const auto err = parse_components(node, map, tileset.context);
        err != parse_error::none) {
      return err;
    }

    return parse_error::none;
  }
  catch (...) {
    return parse_error::could_not_read_external_tileset;
  }
}

}  // namespace

auto parse_tilesets(const YAML::Node& sequence,
                    ir::map_data& map,
                    const std::filesystem::path& dir) -> parse_error
{
  map.tilesets.reserve(sequence.size());

  for (const auto& node : sequence) {
    auto first = node["first-global-id"];
    auto path = node["path"];

    if (!first) {
      return parse_error::no_tileset_first_tile_id;
    }

    if (!path) {
      return parse_error::no_external_tileset_path;
    }

    const auto source = std::filesystem::weakly_canonical(dir / path.as<std::string>());

    if (std::filesystem::exists(source)) {
      if (const auto err = _parse_tileset(source, map, first.as<tile_id>());
          err != parse_error::none) {
        return err;
      }
    }
    else {
      return parse_error::external_tileset_does_not_exist;
    }
  }

  return parse_error::none;
}

}  // namespace tactile::parsing
