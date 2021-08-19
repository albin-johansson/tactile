#include "parse_tilesets.hpp"

#include <cstring>     // strcmp
#include <filesystem>  // exists, weakly_canonical
#include <string>      // string
#include <utility>     // move

#include "parse_properties.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseFirstTileId(const pugi::xml_node node, tile_id& firstId)
    -> ParseError
{
  if (const auto id = GetInt(node, "firstgid"))
  {
    firstId = tile_id{*id};
    return ParseError::None;
  }
  else
  {
    return ParseError::TilesetMissingFirstGid;
  }
}

[[nodiscard]] auto GetTilesetImageRelativePath(const pugi::xml_node node)
    -> Maybe<std::filesystem::path>
{
  const auto tilesetNode = (std::strcmp(node.name(), "tileset") == 0)
                               ? node
                               : node.find_node([](const pugi::xml_node& node) {
                                   return std::strcmp(node.name(), "tileset") == 0;
                                 });

  const auto imageNode = tilesetNode.child("image");
  const auto source = imageNode.attribute("source");

  if (const auto* str = source.as_string(nullptr))
  {
    return std::filesystem::path{str};
  }
  else
  {
    return nothing;
  }
}

[[nodiscard]] auto ParseTiles(const pugi::xml_node node, TilesetData& data)
    -> ParseError
{
  for (const auto tile : node.children("tile"))
  {
    auto& tileData = data.tiles.emplace_back();
    tileData.id = tile_id{GetInt(tile, "id").value()};

    for (const auto frame : tile.child("animation").children("frame"))
    {
      auto& frameData = tileData.animation.emplace_back();
      frameData.tile = tile_id{GetInt(frame, "tileid").value()};
      frameData.duration = GetInt(frame, "duration").value();
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseCommon(const pugi::xml_node node,
                               TilesetData& data,
                               const std::filesystem::path& directory) -> ParseError
{
  if (const auto tileWidth = GetInt(node, "tilewidth"))
  {
    data.tile_width = *tileWidth;
  }
  else
  {
    return ParseError::TilesetMissingTileWidth;
  }

  if (const auto tileHeight = GetInt(node, "tileheight"))
  {
    data.tile_height = *tileHeight;
  }
  else
  {
    return ParseError::TilesetMissingTileHeight;
  }

  const auto relativeImagePath = GetTilesetImageRelativePath(node);
  if (!relativeImagePath)
  {
    return ParseError::TilesetMissingImagePath;
  }

  const auto absoluteImagePath =
      std::filesystem::weakly_canonical(directory / *relativeImagePath);
  if (std::filesystem::exists(absoluteImagePath))
  {
    data.absolute_image_path = absoluteImagePath;
  }
  else
  {
    return ParseError::TilesetImageDoesNotExist;
  }

  if (auto name = GetString(node, "name"))
  {
    data.name = std::move(*name);
  }
  else
  {
    return ParseError::TilesetMissingName;
  }

  if (const auto err = ParseTiles(node, data); err != ParseError::None)
  {
    return err;
  }

  if (const auto err = ParseProperties(node, data.properties);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseExternal(const pugi::xml_node node,
                                 TilesetData& data,
                                 const std::filesystem::path& directory)
    -> ParseError
{
  assert(Contains(node, "source"));

  const auto source = GetString(node, "source").value();
  const auto path = std::filesystem::weakly_canonical(directory / source);

  pugi::xml_document external;
  if (!external.load_file(path.c_str()))
  {
    return ParseError::CouldNotReadExternalTileset;
  }

  return ParseCommon(external.child("tileset"), data, directory);
}

[[nodiscard]] auto ParseTileset(const pugi::xml_node node,
                                TilesetData& data,
                                const std::filesystem::path& directory) -> ParseError
{
  if (const auto err = ParseFirstTileId(node, data.first_id);
      err != ParseError::None)
  {
    return err;
  }

  if (Contains(node, "source"))
  {
    return ParseExternal(node, data, directory);
  }
  else
  {
    return ParseCommon(node, data, directory);
  }

  return ParseError::None;
}

}  // namespace

auto ParseTilesets(const pugi::xml_node root,
                   std::vector<TilesetData>& tilesets,
                   const std::filesystem::path& directory) -> ParseError
{
  for (const auto tileset : root.children("tileset"))
  {
    auto& tilesetData = tilesets.emplace_back();
    if (const auto err = ParseTileset(tileset, tilesetData, directory);
        err != ParseError::None)
    {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
