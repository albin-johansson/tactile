#include <cassert>     // assert
#include <filesystem>  // absolute, exists, weakly_canonical
#include <string>      // string
#include <utility>     // move

#include <nlohmann/json.hpp>  // json
#include <tactile_def.hpp>

#include "../json_common.hpp"
#include "parse_layers.hpp"
#include "parse_properties.hpp"
#include "parse_tilesets.hpp"
#include "parser.hpp"
#include "read_json.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto CheckOrientation(const JSON& json) -> ParseError
{
  const auto it = json.find("orientation");
  if (it != json.end() && it->get<std::string>() == "orthogonal") {
    return ParseError::None;
  }
  else {
    return ParseError::MapUnsupportedOrientation;
  }
}

[[nodiscard]] auto CheckInfinite(const JSON& json) -> ParseError
{
  const auto it = json.find("infinite");
  if (it != json.end() && it->get<bool>()) {
    return ParseError::MapUnsupportedInfinite;
  }
  else {
    return ParseError::None;
  }
}

[[nodiscard]] auto ParseNextLayerId(const JSON& json, Map& map) -> ParseError
{
  if (const auto it = json.find("nextlayerid"); it != json.end()) {
    SetNextLayerId(map, it->get<int32>());
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingNextLayerId;
  }
}

[[nodiscard]] auto ParseNextObjectId(const JSON& json, Map& map) -> ParseError
{
  if (const auto it = json.find("nextobjectid"); it != json.end()) {
    SetNextObjectId(map, it->get<int32>());
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingNextObjectId;
  }
}

[[nodiscard]] auto ParseTileWidth(const JSON& json, Map& map) -> ParseError
{
  if (const auto it = json.find("tilewidth"); it != json.end()) {
    SetTileWidth(map, it->get<int32>());
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingTileWidth;
  }
}

[[nodiscard]] auto ParseTileHeight(const JSON& json, Map& map) -> ParseError
{
  if (const auto it = json.find("tileheight"); it != json.end()) {
    SetTileHeight(map, it->get<int32>());
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingTileHeight;
  }
}

[[nodiscard]] auto ParseWidth(const JSON& json, Map& map) -> ParseError
{
  if (const auto it = json.find("width"); it != json.end()) {
    SetColumnCount(map, it->get<int32>());
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingWidth;
  }
}

[[nodiscard]] auto ParseHeight(const JSON& json, Map& map) -> ParseError
{
  if (const auto it = json.find("height"); it != json.end()) {
    SetRowCount(map, it->get<int32>());
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingHeight;
  }
}

[[nodiscard]] auto ParseMap(const std::filesystem::path& path, Map& map) -> ParseError
{
  const auto absPath = std::filesystem::absolute(path);
  const auto dir = absPath.parent_path();

  if (!std::filesystem::exists(absPath)) {
    return ParseError::MapDoesNotExist;
  }

  SetPath(map, absPath.c_str());

  const auto json = ReadJson(absPath);
  if (!json) {
    return ParseError::CouldNotReadFile;
  }

  if (const auto err = CheckOrientation(*json); err != ParseError::None) {
    return err;
  }

  if (const auto err = CheckInfinite(*json); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseNextLayerId(*json, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseNextObjectId(*json, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseTileWidth(*json, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseTileHeight(*json, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseWidth(*json, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseHeight(*json, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseTilesets(*json, map, dir); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseLayers(*json, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseProperties(*json, map); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseJsonMap(const CPathStr path, ParseError* error) -> Map*
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
