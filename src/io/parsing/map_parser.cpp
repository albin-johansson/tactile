#include "map_parser.hpp"

#include "io/parsing/json/json_map_file.hpp"
#include "io/parsing/parse_layers.hpp"
#include "io/parsing/parse_properties.hpp"
#include "io/parsing/parse_tilesets.hpp"
#include "io/parsing/xml/xml_map_file.hpp"

namespace Tactile::IO {

MapParser::MapParser(const std::filesystem::path& path)
{
#ifndef NDEBUG
  const auto timeStart = cen::counter::now();
#endif  // NDEBUG

  CENTURION_LOG_INFO("Parsing map at \"%s\"...", path.string().c_str());
  mData.absolute_path = std::filesystem::absolute(path);

  if (!std::filesystem::exists(mData.absolute_path))
  {
    mError = ParseError::MapDoesNotExist;
    return;
  }

  if (const auto file = OpenFile(path))
  {
    const auto map = file->GetMap();

    if (!ParseOrientation(map))
    {
      return;
    }

    if (!ParseNextLayerId(map))
    {
      return;
    }

    if (!ParseNextObjectId(map))
    {
      return;
    }

    if (!ParseTileWidth(map))
    {
      return;
    }

    if (!ParseTileHeight(map))
    {
      return;
    }

    mError = ParseTilesets(*file, mData);
    if (mError != ParseError::None)
    {
      return;
    }

    mError = ParseLayers(*file, mData);
    if (mError != ParseError::None)
    {
      return;
    }

    mError = ParseProperties(*file->GetMap(), mData.properties);
    if (mError != ParseError::None)
    {
      return;
    }
  }

#ifndef NDEBUG
  const auto timeFinish = cen::counter::now();
  const auto diff = static_cast<double>(timeFinish - timeStart);
  const auto freq = static_cast<double>(cen::counter::frequency());
  CENTURION_LOG_DEBUG("Parsed map in %f seconds", diff / freq);
#endif  // NDEBUG
}

auto MapParser::OpenFile(const std::filesystem::path& path) -> Unique<IMapFile>
{
  const auto extension = path.extension();
  if (extension == ".json")
  {
    return std::make_unique<JsonMapFile>(path);
  }
  else if (extension == ".tmx")
  {
    return std::make_unique<XmlMapFile>(path);
  }
  else
  {
    mError = ParseError::MapUnsupportedExtension;
    return nullptr;
  }
}

auto MapParser::ParseOrientation(const IMapFile::Object& map) -> bool
{
  const auto orientation = map->GetString(MapAttribute::Orientation);

  if (!orientation || orientation != "orthogonal")
  {
    mError = ParseError::MapUnsupportedOrientation;
    return false;
  }

  return true;
}

auto MapParser::ParseNextLayerId(const IMapFile::Object& map) -> bool
{
  if (const auto id = map->GetInt(MapAttribute::NextLayerId))
  {
    mData.next_layer_id = layer_id{*id};
    return true;
  }
  else
  {
    mError = ParseError::MapMissingNextLayerId;
    return false;
  }
}

auto MapParser::ParseNextObjectId(const IMapFile::Object& map) -> bool
{
  if (const auto id = map->GetInt(MapAttribute::NextObjectId))
  {
    mData.next_object_id = object_id{*id};
    return true;
  }
  else
  {
    mError = ParseError::MapMissingNextObjectId;
    return false;
  }
}

auto MapParser::ParseTileWidth(const IMapFile::Object& map) -> bool
{
  if (const auto tileWidth = map->GetInt(MapAttribute::TileWidth))
  {
    mData.tile_width = *tileWidth;
    return true;
  }
  else
  {
    mError = ParseError::MapMissingTileWidth;
    return false;
  }
}

auto MapParser::ParseTileHeight(const IMapFile::Object& map) -> bool
{
  if (const auto tileHeight = map->GetInt(MapAttribute::TileHeight))
  {
    mData.tile_height = *tileHeight;
    return true;
  }
  else
  {
    mError = ParseError::MapMissingTileHeight;
    return false;
  }
}

}  // namespace Tactile::IO
