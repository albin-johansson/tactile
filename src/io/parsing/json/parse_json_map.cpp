#include "parse_json_map.hpp"

#include <filesystem>  // absolute, exists, weakly_canonical
#include <fstream>     // ifstream
#include <string>      // string
#include <vector>      // vector

#include "aliases/json.hpp"
#include "parse_layers.hpp"
#include "parse_properties.hpp"
#include "parse_tilesets.hpp"
#include "read_json.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseOrientation(const JSON& json) -> ParseError
{
  const auto it = json.find("orientation");
  if (it != json.end() && it->get<std::string>() == "orthogonal") {
    return ParseError::None;
  }
  else {
    return ParseError::MapUnsupportedOrientation;
  }
}

[[nodiscard]] auto ParseInfinite(const JSON& json) -> ParseError
{
  const auto it = json.find("infinite");
  if (it != json.end() && it->get<bool>()) {
    return ParseError::MapUnsupportedInfinite;
  }
  else {
    return ParseError::None;
  }
}

[[nodiscard]] auto ParseNextLayerId(const JSON& json, LayerID& nextLayerId) -> ParseError
{
  if (const auto it = json.find("nextlayerid"); it != json.end()) {
    nextLayerId = LayerID{it->get<LayerID::value_type>()};
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingNextLayerId;
  }
}

[[nodiscard]] auto ParseNextObjectId(const JSON& json, ObjectID& nextObjectId)
    -> ParseError
{
  if (const auto it = json.find("nextobjectid"); it != json.end()) {
    nextObjectId = ObjectID{it->get<ObjectID::value_type>()};
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingNextObjectId;
  }
}

[[nodiscard]] auto ParseTileWidth(const JSON& json, int& tileWidth) -> ParseError
{
  if (const auto it = json.find("tilewidth"); it != json.end()) {
    it->get_to(tileWidth);
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingTileWidth;
  }
}

[[nodiscard]] auto ParseTileHeight(const JSON& json, int& tileHeight) -> ParseError
{
  if (const auto it = json.find("tileheight"); it != json.end()) {
    it->get_to(tileHeight);
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingTileHeight;
  }
}

[[nodiscard]] auto ParseWidth(const JSON& json, int& width) -> ParseError
{
  if (const auto it = json.find("width"); it != json.end()) {
    it->get_to(width);
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingWidth;
  }
}

[[nodiscard]] auto ParseHeight(const JSON& json, int& height) -> ParseError
{
  if (const auto it = json.find("height"); it != json.end()) {
    it->get_to(height);
    return ParseError::None;
  }
  else {
    return ParseError::MapMissingHeight;
  }
}

}  // namespace

auto ParseJsonMap(const std::filesystem::path& path, MapData& data) -> ParseError
{
  data.absolute_path = std::filesystem::absolute(path);
  if (!std::filesystem::exists(data.absolute_path)) {
    return ParseError::MapDoesNotExist;
  }

  const auto json = ReadJson(data.absolute_path);

  if (const auto err = ParseOrientation(json); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseInfinite(json); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseNextLayerId(json, data.next_layer_id);
      err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseNextObjectId(json, data.next_object_id);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseTileWidth(json, data.tile_width); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseTileHeight(json, data.tile_height); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseWidth(json, data.column_count); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseHeight(json, data.row_count); err != ParseError::None) {
    return err;
  }

  const auto directory = data.absolute_path.parent_path();
  if (const auto err = ParseTilesets(json, data.tilesets, directory);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseLayers(json, data.layers); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseProperties(json, data.properties); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
