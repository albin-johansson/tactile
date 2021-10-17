#include "parse_tileset.hpp"

#include <cassert>     // assert
#include <filesystem>  // exists, weakly_canonical
#include <string>      // string
#include <utility>     // move

#include "parse_fancy_tiles.hpp"
#include "parse_properties.hpp"
#include "read_json.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseFirstTileId(const JSON& json) -> Expected<TileID, ParseError>
{
  if (const auto it = json.find("firstgid"); it != json.end()) {
    return TileID{it->get<TileID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingFirstGid);
  }
}

[[nodiscard]] auto ParseTileWidth(const JSON& json) -> Expected<int, ParseError>
{
  if (const auto it = json.find("tilewidth"); it != json.end()) {
    return it->get<int>();
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingTileWidth);
  }
}

[[nodiscard]] auto ParseTileHeight(const JSON& json) -> Expected<int, ParseError>
{
  if (const auto it = json.find("tileheight"); it != json.end()) {
    return it->get<int>();
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingTileHeight);
  }
}

[[nodiscard]] auto ParseImagePath(const JSON& json, const std::filesystem::path& dir)
    -> Expected<std::filesystem::path, ParseError>
{
  const auto relative = json.find("image");
  if (relative == json.end()) {
    return tl::make_unexpected(ParseError::TilesetMissingImagePath);
  }

  auto absolute = std::filesystem::weakly_canonical(dir / relative->get<std::string>());
  if (std::filesystem::exists(absolute)) {
    return absolute;
  }
  else {
    return tl::make_unexpected(ParseError::TilesetImageDoesNotExist);
  }
}

[[nodiscard]] auto ParseName(const JSON& json) -> Expected<std::string, ParseError>
{
  if (const auto it = json.find("name"); it != json.end()) {
    return it->get<std::string>();
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingName);
  }
}

[[nodiscard]] auto ParseTilesetCommon(const TileID first,
                                      const JSON& json,
                                      const std::filesystem::path& dir)
    -> Expected<TilesetData, ParseError>
{
  TilesetData data;
  data.first_id = first;

  if (auto tileWidth = ParseTileWidth(json)) {
    data.tile_width = *tileWidth;
  }
  else {
    return tl::make_unexpected(tileWidth.error());
  }

  if (auto tileHeight = ParseTileHeight(json)) {
    data.tile_height = *tileHeight;
  }
  else {
    return tl::make_unexpected(tileHeight.error());
  }

  if (const auto it = json.find("imagewidth"); it != json.end()) {
    it->get_to(data.image_width);
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingImageWidth);
  }

  if (const auto it = json.find("tilecount"); it != json.end()) {
    it->get_to(data.tile_count);
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingTileCount);
  }

  if (const auto it = json.find("columns"); it != json.end()) {
    it->get_to(data.column_count);
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingColumnCount);
  }

  if (const auto it = json.find("imageheight"); it != json.end()) {
    it->get_to(data.image_height);
  }
  else {
    return tl::make_unexpected(ParseError::TilesetMissingImageHeight);
  }

  if (auto path = ParseImagePath(json, dir)) {
    data.absolute_image_path = std::move(*path);
  }
  else {
    return tl::make_unexpected(path.error());
  }

  if (auto name = ParseName(json)) {
    data.name = std::move(*name);
  }
  else {
    return tl::make_unexpected(name.error());
  }

  if (auto props = ParseProperties(json)) {
    data.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  if (auto tiles = ParseFancyTiles(json)) {
    data.tiles = std::move(*tiles);
  }
  else {
    return tl::make_unexpected(tiles.error());
  }

  return data;
}

[[nodiscard]] auto ParseExternalTileset(const TileID first,
                                        const JSON& json,
                                        const std::filesystem::path& dir)
    -> Expected<TilesetData, ParseError>
{
  assert(json.contains("source"));

  const auto source = json.at("source").get<std::string>();
  const auto path = std::filesystem::weakly_canonical(dir / source);
  if (!std::filesystem::exists(path)) {
    return tl::make_unexpected(ParseError::ExternalTilesetDoesNotExist);
  }

  try {
    const auto external = ReadJson(path).value();
    return ParseTilesetCommon(first, external, dir);
  }
  catch (...) {
    return tl::make_unexpected(ParseError::CouldNotReadExternalTileset);
  }
}

}  // namespace

auto ParseTileset(const JSON& json, const std::filesystem::path& dir)
    -> Expected<TilesetData, ParseError>
{
  return ParseFirstTileId(json).and_then([&](const TileID first) {
    if (json.contains("source")) {
      return ParseExternalTileset(first, json, dir);
    }
    else {
      return ParseTilesetCommon(first, json, dir);
    }
  });
}

}  // namespace Tactile::IO