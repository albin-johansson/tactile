#include "parse_json_map.hpp"

#include <filesystem>  // absolute, exists, weakly_canonical
#include <string>      // string
#include <utility>     // move

#include "common/expected.hpp"
#include "common/json.hpp"
#include "common/maybe.hpp"
#include "parse_layers.hpp"
#include "parse_properties.hpp"
#include "parse_tilesets.hpp"
#include "read_json.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseOrientation(const JSON& json) -> Maybe<ParseError>
{
  const auto it = json.find("orientation");
  if (it != json.end() && it->get<std::string>() == "orthogonal") {
    return nothing;
  }
  else {
    return ParseError::MapUnsupportedOrientation;
  }
}

[[nodiscard]] auto ParseInfinite(const JSON& json) -> Maybe<ParseError>
{
  const auto it = json.find("infinite");
  if (it != json.end() && it->get<bool>()) {
    return ParseError::MapUnsupportedInfinite;
  }
  else {
    return nothing;
  }
}

[[nodiscard]] auto ParseNextLayerId(const JSON& json) -> Expected<LayerID, ParseError>
{
  if (const auto it = json.find("nextlayerid"); it != json.end()) {
    return LayerID{it->get<LayerID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::MapMissingNextLayerId);
  }
}

[[nodiscard]] auto ParseNextObjectId(const JSON& json) -> Expected<ObjectID, ParseError>
{
  if (const auto it = json.find("nextobjectid"); it != json.end()) {
    return ObjectID{it->get<ObjectID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::MapMissingNextObjectId);
  }
}

[[nodiscard]] auto ParseTileWidth(const JSON& json) -> Expected<int, ParseError>
{
  if (const auto it = json.find("tilewidth"); it != json.end()) {
    return it->get<int>();
  }
  else {
    return tl::make_unexpected(ParseError::MapMissingTileWidth);
  }
}

[[nodiscard]] auto ParseTileHeight(const JSON& json) -> Expected<int, ParseError>
{
  if (const auto it = json.find("tileheight"); it != json.end()) {
    return it->get<int>();
  }
  else {
    return tl::make_unexpected(ParseError::MapMissingTileHeight);
  }
}

[[nodiscard]] auto ParseWidth(const JSON& json) -> Expected<int, ParseError>
{
  if (const auto it = json.find("width"); it != json.end()) {
    return it->get<int>();
  }
  else {
    return tl::make_unexpected(ParseError::MapMissingWidth);
  }
}

[[nodiscard]] auto ParseHeight(const JSON& json) -> Expected<int, ParseError>
{
  if (const auto it = json.find("height"); it != json.end()) {
    return it->get<int>();
  }
  else {
    return tl::make_unexpected(ParseError::MapMissingHeight);
  }
}

}  // namespace

auto ParseJsonMap(const std::filesystem::path& path) -> Expected<MapData, ParseError>
{
  const auto absPath = std::filesystem::absolute(path);
  if (!std::filesystem::exists(absPath)) {
    return tl::make_unexpected(ParseError::MapDoesNotExist);
  }

  const auto json = ReadJson(absPath);
  if (!json) {
    return tl::make_unexpected(ParseError::CouldNotReadFile);
  }

  if (const auto err = ParseOrientation(*json)) {
    return tl::make_unexpected(*err);
  }

  if (const auto err = ParseInfinite(*json)) {
    return tl::make_unexpected(*err);
  }

  MapData data;
  data.absolute_path = absPath;

  if (const auto id = ParseNextLayerId(*json)) {
    data.next_layer_id = *id;
  }
  else {
    return tl::make_unexpected(id.error());
  }

  if (const auto id = ParseNextObjectId(*json)) {
    data.next_object_id = *id;
  }
  else {
    return tl::make_unexpected(id.error());
  }

  if (const auto tileWidth = ParseTileWidth(*json)) {
    data.tile_width = *tileWidth;
  }
  else {
    return tl::make_unexpected(tileWidth.error());
  }

  if (const auto tileHeight = ParseTileHeight(*json)) {
    data.tile_height = *tileHeight;
  }
  else {
    return tl::make_unexpected(tileHeight.error());
  }

  if (const auto width = ParseWidth(*json)) {
    data.column_count = *width;
  }
  else {
    return tl::make_unexpected(width.error());
  }

  if (const auto height = ParseHeight(*json)) {
    data.row_count = *height;
  }
  else {
    return tl::make_unexpected(height.error());
  }

  if (auto tilesets = ParseTilesets(*json, data.absolute_path.parent_path())) {
    data.tilesets = std::move(*tilesets);
  }
  else {
    return tl::make_unexpected(tilesets.error());
  }

  if (auto layers = ParseLayers(*json)) {
    data.layers = std::move(*layers);
  }
  else {
    return tl::make_unexpected(layers.error());
  }

  if (auto props = ParseProperties(*json)) {
    data.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return data;
}

}  // namespace Tactile::IO
