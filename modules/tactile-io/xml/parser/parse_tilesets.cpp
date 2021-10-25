#include "parse_tilesets.hpp"

#include <cassert>     // assert
#include <cstring>     // strcmp
#include <filesystem>  // exists, weakly_canonical
#include <string>      // string

#include "parse_fancy_tiles.hpp"
#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto GetImageNode(const pugi::xml_node node) -> pugi::xml_node
{
  const auto tilesetNode = (std::strcmp(node.name(), "tileset") == 0)
                               ? node
                               : node.find_node([](const pugi::xml_node& node) {
                                   return std::strcmp(node.name(), "tileset") == 0;
                                 });
  return tilesetNode.child("image");
}

[[nodiscard]] auto GetTilesetImageRelativePath(const pugi::xml_node node)
    -> Maybe<std::filesystem::path>
{
  const auto source = node.attribute("source");
  if (const auto* str = source.as_string(nullptr)) {
    return std::filesystem::path{str};
  }
  else {
    return nothing;
  }
}

[[nodiscard]] auto ParseCommon(const pugi::xml_node node,
                               Tileset& tileset,
                               const std::filesystem::path& dir) -> ParseError
{
  if (const auto tileWidth = GetInt(node, "tilewidth")) {
    IO::SetTileWidth(tileset, *tileWidth);
  }
  else {
    return ParseError::TilesetMissingTileWidth;
  }

  if (const auto tileHeight = GetInt(node, "tileheight")) {
    IO::SetTileHeight(tileset, *tileHeight);
  }
  else {
    return ParseError::TilesetMissingTileHeight;
  }

  if (const auto count = GetInt(node, "tilecount")) {
    IO::SetTileCount(tileset, *count);
  }
  else {
    return ParseError::TilesetMissingTileCount;
  }

  if (const auto count = GetInt(node, "columns")) {
    IO::SetColumnCount(tileset, *count);
  }
  else {
    return ParseError::TilesetMissingColumnCount;
  }

  const auto imageNode = GetImageNode(node);

  if (const auto imageWidth = GetInt(imageNode, "width")) {
    IO::SetImageWidth(tileset, *imageWidth);
  }
  else {
    return ParseError::TilesetMissingImageWidth;
  }

  if (const auto imageHeight = GetInt(imageNode, "height")) {
    IO::SetImageHeight(tileset, *imageHeight);
  }
  else {
    return ParseError::TilesetMissingImageHeight;
  }

  const auto relImagePath = GetTilesetImageRelativePath(imageNode);
  if (!relImagePath) {
    return ParseError::TilesetMissingImagePath;
  }

  const auto absoluteImagePath = std::filesystem::weakly_canonical(dir / *relImagePath);
  if (std::filesystem::exists(absoluteImagePath)) {
    IO::SetImagePath(tileset, absoluteImagePath.c_str());
  }
  else {
    return ParseError::TilesetImageDoesNotExist;
  }

  if (auto name = GetString(node, "name")) {
    IO::SetName(tileset, name->c_str());
  }
  else {
    return ParseError::TilesetMissingName;
  }

  if (const auto err = ParseFancyTiles(node, tileset); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseProperties(node, tileset); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseExternal(const pugi::xml_node node,
                                 Tileset& tileset,
                                 const std::filesystem::path& dir) -> ParseError
{
  assert(Contains(node, "source"));

  const auto source = GetString(node, "source").value();
  const auto path = std::filesystem::weakly_canonical(dir / source);
  if (!std::filesystem::exists(path)) {
    return ParseError::ExternalTilesetDoesNotExist;
  }

  pugi::xml_document external;
  if (!external.load_file(path.c_str())) {
    return ParseError::CouldNotReadExternalTileset;
  }

  return ParseCommon(external.child("tileset"), tileset, dir);
}

[[nodiscard]] auto ParseTileset(const pugi::xml_node node,
                                Tileset& tileset,
                                const std::filesystem::path& dir) -> ParseError
{
  if (const auto id = GetInt(node, "firstgid")) {
    IO::SetFirstGlobalId(tileset, *id);
  }
  else {
    return ParseError::TilesetMissingFirstGid;
  }

  if (Contains(node, "source")) {
    return ParseExternal(node, tileset, dir);
  }
  else {
    return ParseCommon(node, tileset, dir);
  }
}

}  // namespace

auto ParseTilesets(const pugi::xml_node root, Map& map, const std::filesystem::path& dir)
    -> ParseError
{
  for (const auto tilesetNode : root.children("tileset")) {
    auto& tileset = IO::AddTileset(map);
    if (const auto err = ParseTileset(tilesetNode, tileset, dir); err != ParseError::None)
    {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
