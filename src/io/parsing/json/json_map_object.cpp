#include "json_map_object.hpp"

#include <cassert>  // assert
#include <fstream>  // ifstream
#include <utility>  // move

#include "core/map/map_position.hpp"
#include "core/tactile_error.hpp"

namespace Tactile::IO {
namespace {

template <typename T>
[[nodiscard]] auto Get(const nlohmann::json& json, const std::string_view key)
    -> Maybe<T>
{
  if (const auto it = json.find(key); it != json.end())
  {
    return it->get<T>();
  }
  else
  {
    return nothing;
  }
}

}  // namespace

JsonMapObject::JsonMapObject(const std::filesystem::path& path)
{
  std::ifstream stream{path};
  stream >> mJson;
}

JsonMapObject::JsonMapObject(nlohmann::json json) : mJson(std::move(json))
{}

auto JsonMapObject::FromFile(const std::filesystem::path& path) const -> Object
{
  try
  {
    return std::make_unique<JsonMapObject>(path);
  }
  catch (...)
  {
    return nullptr;
  }
}

auto JsonMapObject::Contains(const MapAttribute attr) const -> bool
{
  const auto key = ToKey(attr);
  return mJson.find(key) != mJson.end();
}

auto JsonMapObject::GetInt(const MapAttribute attr) const -> Maybe<int>
{
  const auto key = ToKey(attr);
  return Get<int>(mJson, key);
}

auto JsonMapObject::GetFloat(const MapAttribute attr) const -> Maybe<float>
{
  const auto key = ToKey(attr);
  return Get<float>(mJson, key);
}

auto JsonMapObject::GetBool(const MapAttribute attr) const -> Maybe<bool>
{
  const auto key = ToKey(attr);
  return Get<bool>(mJson, key);
}

auto JsonMapObject::GetString(const MapAttribute attr) const -> Maybe<std::string>
{
  const auto key = ToKey(attr);
  return Get<std::string>(mJson, key);
}

auto JsonMapObject::GetArray(const MapAttribute attr) const -> Objects
{
  const auto key = ToKey(attr);
  return GetArray(key);
}

auto JsonMapObject::GetArray(const std::string_view key) const -> Objects
{
  Objects objects;

  if (const auto it = mJson.find(key); it != mJson.end() && it->is_array())
  {
    for (const auto& [key, value] : it->items())
    {
      assert(value.is_object());
      objects.push_back(std::make_unique<JsonMapObject>(value));
    }
  }

  return objects;
}

auto JsonMapObject::GetTilesetImageRelativePath() const
    -> Maybe<std::filesystem::path>
{
  return GetString(MapAttribute::Image);
}

auto JsonMapObject::IsTileLayer() const -> bool
{
  assert(mJson.contains("type"));
  return mJson.at("type") == "tilelayer";
}

auto JsonMapObject::IsObjectLayer() const -> bool
{
  assert(mJson.contains("type"));
  return mJson.at("type") == "objectgroup";
}

auto JsonMapObject::IsImplicitStringProperty() const -> bool
{
  return false;
}

auto JsonMapObject::IsPoint() const -> bool
{
  return mJson.contains("point");  // The value of "point" is always true
}

auto JsonMapObject::IsEllipse() const -> bool
{
  return mJson.contains("ellipse");  // The value of "ellipse" is always true
}

auto JsonMapObject::HasType() const -> bool
{
  return mJson.contains("type");
}

auto JsonMapObject::GetTileData(const col_t nCols, TileMatrix& matrix) const
    -> ParseError
{
  assert(IsTileLayer());

  // For now, only support the CSV tile encoding, which is the implicit Tiled default
  if (const auto it = mJson.find("encoding"); it != mJson.end())
  {
    if (it->get<std::string>() != "csv")
    {
      return ParseError::UnsupportedTileEncoding;
    }
  }

  const auto data = mJson.at("data");
  if (!data.is_array())
  {
    return ParseError::CouldNotParseTiles;
  }

  for (int index = 0; const auto& elem : data.items())
  {
    const auto value = elem.value();
    if (value.is_number_integer())
    {
      const auto id = tile_id{value.get<int>()};
      const auto pos = MapPosition::FromIndex(index, nCols);
      matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = id;
    }
    else
    {
      return ParseError::CouldNotParseTiles;
    }

    ++index;
  }

  return ParseError::None;
}

auto JsonMapObject::GetObjects() const -> Objects
{
  assert(IsObjectLayer());
  return GetArray("objects");
}

auto JsonMapObject::GetProperties() const -> IMapObject::Objects
{
  return GetArray("properties");
}

auto JsonMapObject::GetExternalTileset() const -> Object
{
  return std::make_unique<JsonMapObject>(mJson);
}

auto JsonMapObject::ToKey(const MapAttribute attr) -> std::string_view
{
  switch (attr)
  {
    case MapAttribute::Tileset:
      return "tileset";

    case MapAttribute::NextLayerId:
      return "nextlayerid";

    case MapAttribute::NextObjectId:
      return "nextobjectid";

    case MapAttribute::Source:
      return "source";

    case MapAttribute::FirstGid:
      return "firstgid";

    case MapAttribute::TileWidth:
      return "tilewidth";

    case MapAttribute::TileHeight:
      return "tileheight";

    case MapAttribute::Image:
      return "image";

    case MapAttribute::Name:
      return "name";

    case MapAttribute::Id:
      return "id";

    case MapAttribute::Width:
      return "width";

    case MapAttribute::Height:
      return "height";

    case MapAttribute::Visible:
      return "visible";

    case MapAttribute::Opacity:
      return "opacity";

    case MapAttribute::Type:
      return "type";

    case MapAttribute::Value:
      return "value";

    case MapAttribute::X:
      return "x";

    case MapAttribute::Y:
      return "y";

    case MapAttribute::Point:
      return "point";

    case MapAttribute::Orientation:
      return "orientation";

    case MapAttribute::Infinite:
      return "infinite";

    default:
      throw TactileError{"Invalid JSON map file attribute!"};
  }
}

}  // namespace Tactile::IO
