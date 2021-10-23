#include <cassert>     // assert
#include <filesystem>  // absolute

#include <yaml-cpp/yaml.h>

#include "parse_layers.hpp"
#include "parse_properties.hpp"
#include "parse_tilesets.hpp"
#include "parser.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseMap(const std::filesystem::path& path, Map& map) -> ParseError
{
  try {
    const auto node = YAML::LoadFile(path.string());
    if (!node) {
      return ParseError::CouldNotReadFile;
    }

    SetPath(map, path.c_str());

    if (auto nRows = node["row-count"]) {
      SetRowCount(map, nRows.as<int32>());
    }
    else {
      return ParseError::MapMissingHeight;
    }

    if (auto nCols = node["column-count"]) {
      SetColumnCount(map, nCols.as<int32>());
    }
    else {
      return ParseError::MapMissingWidth;
    }

    if (auto tileWidth = node["tile-width"]) {
      SetTileWidth(map, tileWidth.as<int32>());
    }
    else {
      return ParseError::MapMissingTileWidth;
    }

    if (auto tileHeight = node["tile-height"]) {
      SetTileHeight(map, tileHeight.as<int32>());
    }
    else {
      return ParseError::MapMissingTileHeight;
    }

    if (auto id = node["next-layer-id"]) {
      SetNextLayerId(map, id.as<int32>());
    }
    else {
      return ParseError::MapMissingNextLayerId;
    }

    if (auto id = node["next-object-id"]) {
      SetNextObjectId(map, id.as<int32>());
    }
    else {
      return ParseError::MapMissingNextObjectId;
    }

    if (auto seq = node["layers"]) {
      if (const auto err = ParseLayers(seq, map); err != ParseError::None) {
        return err;
      }
    }

    if (auto seq = node["tilesets"]) {
      if (const auto err = ParseTilesets(seq, map); err != ParseError::None) {
        return err;
      }
    }

    if (const auto err = ParseProperties(node, map); err != ParseError::None) {
      return err;
    }

    return ParseError::None;
  }
  catch (...) {
    return ParseError::Unknown;
  }
}

}  // namespace

auto ParseYamlMap(const CPathStr path, ParseError* error) -> Map*
{
  assert(path);

  auto map = CreateMap();
  const auto err = ParseMap(path, *map);

  if (error) {
    *error = err;
  }

  if (err == ParseError::None) {
    return map.release();
  }
  else {
    return nullptr;
  }
}

}  // namespace Tactile::IO
