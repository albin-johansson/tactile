#include <filesystem>  // absolute
#include <utility>     // move

#include "parse_layers.hpp"
#include "parse_properties.hpp"
#include "parse_tilesets.hpp"
#include "parser.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseMap(const std::filesystem::path& path)
    -> tl::expected<MapData, ParseError>
{
  try {
    const auto node = YAML::LoadFile(path.string());
    if (!node) {
      return tl::make_unexpected(ParseError::CouldNotReadFile);
    }

    MapData map;
    map.absolute_path = std::filesystem::absolute(path);

    if (auto nRows = node["row-count"]) {
      map.row_count = nRows.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::MapMissingHeight);
    }

    if (auto nCols = node["column-count"]) {
      map.column_count = nCols.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::MapMissingWidth);
    }

    if (auto tileWidth = node["tile-width"]) {
      map.tile_width = tileWidth.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::MapMissingTileWidth);
    }

    if (auto tileHeight = node["tile-height"]) {
      map.tile_height = tileHeight.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::MapMissingTileHeight);
    }

    if (auto id = node["next-layer-id"]) {
      map.next_layer_id = LayerID{id.as<LayerID::value_type>()};
    }
    else {
      return tl::make_unexpected(ParseError::MapMissingNextLayerId);
    }

    if (auto id = node["next-object-id"]) {
      map.next_object_id = ObjectID{id.as<ObjectID::value_type>()};
    }
    else {
      return tl::make_unexpected(ParseError::MapMissingNextObjectId);
    }

    if (auto seq = node["layers"]) {
      if (auto layers = ParseLayers(seq, map.row_count, map.column_count)) {
        map.layers = std::move(*layers);
      }
      else {
        return tl::make_unexpected(layers.error());
      }
    }

    if (auto seq = node["tilesets"]) {
      if (auto tilesets = ParseTilesets(seq, map.absolute_path.parent_path())) {
        map.tilesets = std::move(*tilesets);
      }
      else {
        return tl::make_unexpected(tilesets.error());
      }
    }

    if (auto props = ParseProperties(node)) {
      map.properties = std::move(*props);
    }
    else {
      return tl::make_unexpected(props.error());
    }

    return map;
  }
  catch (...) {
    return tl::make_unexpected(ParseError::Unknown);
  }
}

}  // namespace

auto ParseYamlMap(const std::filesystem::path& path, ParseError* error)
    -> std::optional<MapData>
{
  if (auto map = ParseMap(path)) {
    if (error) {
      *error = ParseError::None;
    }
    return std::move(map.value());
  }
  else {
    if (error) {
      *error = map.error();
    }
    return std::nullopt;
  }
}

}  // namespace Tactile::IO
