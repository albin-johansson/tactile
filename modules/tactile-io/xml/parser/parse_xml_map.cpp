#include <cstring>     // strcmp
#include <filesystem>  // absolute, exists

#include <pugixml.hpp>  // xml_document, xml_node

#include "parse_layers.hpp"
#include "parse_properties.hpp"
#include "parse_tilesets.hpp"
#include "parser.hpp"
#include "xml_utils.hpp"

namespace tactile::IO {
namespace {

[[nodiscard]] auto IsOrthogonalMap(const pugi::xml_node root) -> bool
{
  const auto orientation = GetString(root, "orientation");
  return orientation && orientation == "orthogonal";
}

[[nodiscard]] auto IsInfiniteMap(const pugi::xml_node root) -> bool
{
  const auto infinite = root.attribute("infinite");
  return !infinite.empty() && infinite.as_bool(true);
}

[[nodiscard]] auto ParseMap(const std::filesystem::path& path, Map& map) -> ParseError
{
  const auto absolute = std::filesystem::absolute(path);

  if (!std::filesystem::exists(absolute)) {
    return ParseError::MapDoesNotExist;
  }

  SetPath(map, absolute.c_str());

  pugi::xml_document document;
  if (!document.load_file(path.c_str())) {
    return ParseError::CouldNotReadFile;
  }

  const auto root = document.child("map");

  if (!IsOrthogonalMap(root)) {
    return ParseError::MapUnsupportedOrientation;
  }

  if (IsInfiniteMap(root)) {
    return ParseError::MapUnsupportedInfinite;
  }

  if (const auto value = GetInt(root, "nextlayerid")) {
    SetNextLayerId(map, *value);
  }
  else {
    return ParseError::MapMissingNextLayerId;
  }

  if (const auto value = GetInt(root, "nextobjectid")) {
    SetNextObjectId(map, *value);
  }
  else {
    return ParseError::MapMissingNextObjectId;
  }

  if (const auto value = GetInt(root, "tilewidth")) {
    SetTileWidth(map, *value);
  }
  else {
    return ParseError::MapMissingTileWidth;
  }

  if (const auto value = GetInt(root, "tileheight")) {
    SetTileHeight(map, *value);
  }
  else {
    return ParseError::MapMissingTileHeight;
  }

  if (const auto value = GetInt(root, "height")) {
    SetRowCount(map, *value);
  }
  else {
    return ParseError::MapMissingHeight;
  }

  if (const auto value = GetInt(root, "width")) {
    SetColumnCount(map, *value);
  }
  else {
    return ParseError::MapMissingWidth;
  }

  const auto dir = absolute.parent_path();
  if (const auto err = ParseTilesets(root, map, dir); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseLayers(root, map); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseProperties(root, map); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseXmlMap(const CPathStr path, ParseError* error) -> Map*
{
  auto data = CreateMap();
  const auto err = ParseMap(path, *data);

  if (error) {
    *error = err;
  }

  if (err == ParseError::None) {
    return data.release();
  }
  else {
    return nullptr;
  }
}

}  // namespace tactile::IO
