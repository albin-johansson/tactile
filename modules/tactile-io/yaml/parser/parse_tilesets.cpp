#include "parse_tilesets.hpp"

#include <filesystem>  // exists, weakly_canonical
#include <string>      // string

#include <yaml-cpp/yaml.h>

#include "parse_components.hpp"
#include "parse_fancy_tiles.hpp"
#include "parse_properties.hpp"

namespace tactile::IO {
namespace {

[[nodiscard]] auto ParseTileset(Map& map,
                                const int32 first,
                                const std::filesystem::path& path) -> ParseError
{
  try {
    const auto node = YAML::LoadFile(path.string());
    if (!node) {
      return ParseError::CouldNotReadExternalTileset;
    }

    auto& tileset = AddTileset(map);
    SetFirstGlobalId(tileset, first);

    if (auto version = node["version"]) {
      if (version.as<int32>() != 1) {
        return ParseError::TilesetInvalidVersion;
      }
    }
    else {
      return ParseError::TilesetMissingVersion;
    }

    if (auto name = node["name"]) {
      SetName(tileset, name.as<std::string>().c_str());
    }
    else {
      return ParseError::TilesetMissingName;
    }

    if (auto count = node["tile-count"]) {
      SetTileCount(tileset, count.as<int32>());
    }
    else {
      return ParseError::TilesetMissingTileCount;
    }

    if (auto tileWidth = node["tile-width"]) {
      SetTileWidth(tileset, tileWidth.as<int32>());
    }
    else {
      return ParseError::TilesetMissingTileWidth;
    }

    if (auto tileHeight = node["tile-height"]) {
      SetTileHeight(tileset, tileHeight.as<int32>());
    }
    else {
      return ParseError::TilesetMissingTileHeight;
    }

    if (auto columns = node["column-count"]) {
      SetColumnCount(tileset, columns.as<int32>());
    }
    else {
      return ParseError::TilesetMissingColumnCount;
    }

    if (auto rel = node["image-path"]) {
      const auto dir = path.parent_path();
      auto abs = std::filesystem::weakly_canonical(dir / rel.as<std::string>());
      if (std::filesystem::exists(abs)) {
        SetImagePath(tileset, abs.c_str());
      }
      else {
        return ParseError::TilesetImageDoesNotExist;
      }
    }
    else {
      return ParseError::TilesetMissingImagePath;
    }

    if (auto imageWidth = node["image-width"]) {
      SetImageWidth(tileset, imageWidth.as<int32>());
    }
    else {
      return ParseError::TilesetMissingImageWidth;
    }

    if (auto imageHeight = node["image-height"]) {
      SetImageHeight(tileset, imageHeight.as<int32>());
    }
    else {
      return ParseError::TilesetMissingImageHeight;
    }

    if (const auto err = ParseFancyTiles(node, map, tileset); err != ParseError::None) {
      return err;
    }

    if (const auto err = ParseProperties(node, tileset); err != ParseError::None) {
      return err;
    }

    if (const auto err = ParseComponents(map, node, tileset); err != ParseError::None) {
      return err;
    }

    return ParseError::None;
  }
  catch (...) {
    return ParseError::CouldNotReadExternalTileset;
  }
}

}  // namespace

auto ParseTilesets(const YAML::Node& seq, Map& map) -> ParseError
{
  ReserveTilesets(map, seq.size());

  for (const auto& tilesetRef : seq) {
    auto first = tilesetRef["first-global-id"];
    auto path = tilesetRef["path"];

    if (!first) {
      return ParseError::TilesetMissingFirstGid;
    }

    if (!path) {
      return ParseError::MissingTilesetPath;
    }

    const auto source = path.as<std::string>();
    const auto dir = std::filesystem::path(GetPath(map)).parent_path();
    const auto definition = std::filesystem::weakly_canonical(dir / source);

    if (std::filesystem::exists(definition)) {
      if (const auto err = ParseTileset(map, first.as<int32>(), definition);
          err != ParseError::None) {
        return err;
      }
    }
    else {
      return ParseError::ExternalTilesetDoesNotExist;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::IO
