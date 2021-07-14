#include "map_parser.hpp"

#include "io/parsing/json/json_map_file.hpp"
#include "io/parsing/parse_layers.hpp"
#include "io/parsing/parse_properties.hpp"
#include "io/parsing/parse_tilesets.hpp"
#include "io/parsing/xml/xml_map_file.hpp"

namespace Tactile::IO {

MapParser::MapParser(const std::filesystem::path& path)
{
  CENTURION_LOG_INFO("Parsing map at \"%s\"...", path.string().c_str());
  mData.absolute_path = std::filesystem::absolute(path);

  if (!std::filesystem::exists(mData.absolute_path))
  {
    mError = ParseError::MapDoesNotExist;
    return;
  }

  if (const auto file = OpenFile(path))
  {
    if (!ParseNextLayerId(*file))
    {
      return;
    }

    if (!ParseNextObjectId(*file))
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

auto MapParser::ParseNextLayerId(const IMapFile& file) -> bool
{
  const auto map = file.GetMap();
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

auto MapParser::ParseNextObjectId(const IMapFile& file) -> bool
{
  const auto map = file.GetMap();
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

}  // namespace Tactile::IO
