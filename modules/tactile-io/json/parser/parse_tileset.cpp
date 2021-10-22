#include "parse_tileset.hpp"

#include <cassert>     // assert
#include <filesystem>  // exists, weakly_canonical
#include <string>      // string

#include "parse_fancy_tiles.hpp"
#include "parse_properties.hpp"
#include "read_json.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseImagePath(const JSON& json,
                                  Tileset& tileset,
                                  const std::filesystem::path& dir) -> ParseError
{
  const auto relative = json.find("image");
  if (relative == json.end()) {
    return ParseError::TilesetMissingImagePath;
  }

  auto absolute = std::filesystem::weakly_canonical(dir / relative->get<std::string>());
  if (std::filesystem::exists(absolute)) {
    SetImagePath(tileset, absolute.c_str());
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseTilesetCommon(const JSON& json,
                                      Tileset& tileset,
                                      const std::filesystem::path& dir) -> ParseError
{
  if (const auto it = json.find("name"); it != json.end()) {
    SetName(tileset, it->get<std::string>().c_str());
  }
  else {
    return ParseError::TilesetMissingName;
  }

  if (const auto it = json.find("tileheight"); it != json.end()) {
    SetTileWidth(tileset, it->get<int32>());
  }
  else {
    return ParseError::TilesetMissingTileHeight;
  }

  if (const auto it = json.find("tileheight"); it != json.end()) {
    SetTileHeight(tileset, it->get<int32>());
  }
  else {
    return ParseError::TilesetMissingTileHeight;
  }

  if (const auto err = ParseImagePath(json, tileset, dir); err != ParseError::None) {
    return err;
  }

  if (const auto it = json.find("imagewidth"); it != json.end()) {
    SetImageWidth(tileset, it->get<int32>());
  }
  else {
    return ParseError::TilesetMissingImageWidth;
  }

  if (const auto it = json.find("imageheight"); it != json.end()) {
    SetImageHeight(tileset, it->get<int32>());
  }
  else {
    return ParseError::TilesetMissingImageHeight;
  }

  if (const auto it = json.find("tilecount"); it != json.end()) {
    SetTileCount(tileset, it->get<int32>());
  }
  else {
    return ParseError::TilesetMissingTileCount;
  }

  if (const auto it = json.find("columns"); it != json.end()) {
    SetColumnCount(tileset, it->get<int32>());
  }
  else {
    return ParseError::TilesetMissingColumnCount;
  }

  if (const auto err = ParseProperties(json, tileset); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseFancyTiles(json, tileset); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseExternalTileset(const JSON& json,
                                        Tileset& tileset,
                                        const std::filesystem::path& dir) -> ParseError
{
  assert(json.contains("source"));

  const auto source = json.at("source").get<std::string>();
  const auto path = std::filesystem::weakly_canonical(dir / source);
  if (!std::filesystem::exists(path)) {
    return ParseError::ExternalTilesetDoesNotExist;
  }

  try {
    const auto external = ReadJson(path).value();
    return ParseTilesetCommon(external, tileset, dir);
  }
  catch (...) {
    return ParseError::CouldNotReadExternalTileset;
  }
}

}  // namespace

auto ParseTileset(const JSON& json, Tileset& tileset, const std::filesystem::path& dir)
    -> ParseError
{
  if (const auto it = json.find("firstgid"); it != json.end()) {
    SetFirstGlobalId(tileset, it->get<int32>());
  }
  else {
    return ParseError::TilesetMissingFirstGid;
  }

  if (json.contains("source")) {
    return ParseExternalTileset(json, tileset, dir);
  }
  else {
    return ParseTilesetCommon(json, tileset, dir);
  }
}

}  // namespace Tactile::IO