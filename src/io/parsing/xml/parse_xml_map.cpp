#include "parse_xml_map.hpp"

#include <cstring>      // strcmp
#include <filesystem>   // absolute, exists
#include <pugixml.hpp>  // xml_document, xml_node

#include "parse_layers.hpp"
#include "parse_properties.hpp"
#include "parse_tilesets.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseOrientation(const pugi::xml_node root) -> ParseError
{
  if (const auto orientation = GetString(root, "orientation");
      orientation && orientation != "orthogonal")
  {
    return ParseError::MapUnsupportedOrientation;
  }
  else
  {
    return ParseError::None;
  }
}

[[nodiscard]] auto ParseInfinite(const pugi::xml_node root) -> ParseError
{
  if (const auto infinite = root.attribute("infinite");
      !infinite.empty() && infinite.as_bool(true))
  {
    return ParseError::MapUnsupportedInfinite;
  }
  else
  {
    return ParseError::None;
  }
}

[[nodiscard]] auto ParseNextLayerId(const pugi::xml_node root, LayerID& nextLayerId)
    -> ParseError
{
  if (const auto value = GetInt(root, "nextlayerid"))
  {
    nextLayerId = LayerID{*value};
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingNextLayerId;
  }
}

[[nodiscard]] auto ParseNextObjectId(const pugi::xml_node root,
                                     ObjectID& nextObjectId) -> ParseError
{
  if (const auto value = GetInt(root, "nextobjectid"))
  {
    nextObjectId = ObjectID{*value};
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingNextObjectId;
  }
}

[[nodiscard]] auto ParseTileWidth(const pugi::xml_node root, int& tileWidth)
    -> ParseError
{
  if (const auto value = GetInt(root, "tilewidth"))
  {
    tileWidth = *value;
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingTileWidth;
  }
}

[[nodiscard]] auto ParseTileHeight(const pugi::xml_node root, int& tileHeight)
    -> ParseError
{
  if (const auto value = GetInt(root, "tileheight"))
  {
    tileHeight = *value;
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingTileHeight;
  }
}

[[nodiscard]] auto ParseWidth(const pugi::xml_node root, int& width) -> ParseError
{
  if (const auto value = GetInt(root, "width"))
  {
    width = *value;
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingWidth;
  }
}

[[nodiscard]] auto ParseHeight(const pugi::xml_node root, int& height) -> ParseError
{
  if (const auto value = GetInt(root, "height"))
  {
    height = *value;
    return ParseError::None;
  }
  else
  {
    return ParseError::MapMissingHeight;
  }
}

}  // namespace

auto ParseXmlMap(const std::filesystem::path& path, MapData& data) -> ParseError
{
  data.absolute_path = std::filesystem::absolute(path);
  if (!std::filesystem::exists(data.absolute_path))
  {
    return ParseError::MapDoesNotExist;
  }

  pugi::xml_document document;
  if (!document.load_file(path.c_str()))
  {
    return ParseError::CouldNotReadFile;
  }

  const auto root = document.child("map");

  if (const auto err = ParseOrientation(root); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseInfinite(root); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseNextLayerId(root, data.next_layer_id);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseNextObjectId(root, data.next_object_id);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseTileWidth(root, data.tile_width);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseTileHeight(root, data.tile_height);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseWidth(root, data.column_count); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseHeight(root, data.row_count); err != ParseError::None)
  {
    return err;
  }

  const auto directory = data.absolute_path.parent_path();
  if (const auto err = ParseTilesets(root, data.tilesets, directory);
      err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseLayers(root, data.layers); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseProperties(root, data.properties);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
