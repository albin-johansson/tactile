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
  if (it != json.end() && it->get<std::string>() == "orthogonal")
  {
    return ParseError::None;
  }
  else
  {
    return ParseError::MapUnsupportedOrientation;
  }
}

[[nodiscard]] auto ParseInfinite(const JSON& json) -> ParseError
{
  const auto it = json.find("infinite");
  if (it != json.end() && it->get<bool>())
  {
    return ParseError::MapUnsupportedInfinite;
  }
  else
  {
    return ParseError::None;
  }
}

[[nodiscard]] auto ParseNextLayerId(const JSON& json, layer_id& nextLayerId)
    -> ParseError
{
  if (const auto it = json.find("nextlayerid"); it != json.end())
  {
    nextLayerId = layer_id{it->get<layer_id::value_type>()};
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingNextLayerId;
  }
}

[[nodiscard]] auto ParseNextObjectId(const JSON& json, object_id& nextObjectId)
    -> ParseError
{
  if (const auto it = json.find("nextobjectid"); it != json.end())
  {
    nextObjectId = object_id{it->get<object_id::value_type>()};
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingNextObjectId;
  }
}

[[nodiscard]] auto ParseTileWidth(const JSON& json, int& tileWidth) -> ParseError
{
  if (const auto it = json.find("tilewidth"); it != json.end())
  {
    tileWidth = it->get<int>();
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingTileWidth;
  }
}

[[nodiscard]] auto ParseTileHeight(const JSON& json, int& tileHeight) -> ParseError
{
  if (const auto it = json.find("tileheight"); it != json.end())
  {
    tileHeight = it->get<int>();
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingTileHeight;
  }
}

}  // namespace

auto ParseJsonMap(const std::filesystem::path& path, MapData& data) -> ParseError
{
  data.absolute_path = std::filesystem::absolute(path);
  if (!std::filesystem::exists(data.absolute_path))
  {
    return ParseError::MapDoesNotExist;
  }

  const auto json = ReadJson(data.absolute_path);

  if (const auto err = ParseOrientation(json); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseInfinite(json); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseNextLayerId(json, data.next_layer_id);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseNextObjectId(json, data.next_object_id);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseTileWidth(json, data.tile_width);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseTileHeight(json, data.tile_height);
      err != ParseError::None)
  {
    return err;
  }

  const auto directory = data.absolute_path.parent_path();
  if (const auto err = ParseTilesets(json, data.tilesets, directory);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseLayers(json, data.layers); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseProperties(json, data.properties);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
