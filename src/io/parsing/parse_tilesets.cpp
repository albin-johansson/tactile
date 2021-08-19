#include "parse_tilesets.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "io/parsing/parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseTiles(const IMapObject& object, TilesetData& tileset)
    -> ParseError
{
  for (const auto& tile : object.GetArray(MapAttribute::Tiles))
  {
    auto& tileData = tileset.tiles.emplace_back();

    if (const auto id = tile->GetInt(MapAttribute::Id))
    {
      tileData.id = tile_id{*id};
    }
    else
    {
      return ParseError::Unknown;  // FIXME
    }

    for (const auto& frame : tile->GetArray(MapAttribute::Animation))
    {
      auto& frameData = tileData.animation.emplace_back();

      if (const auto tileId = frame->GetInt(MapAttribute::TileId))
      {
        frameData.tile = tile_id{*tileId};
      }
      else
      {
        return ParseError::Unknown;  // FIXME
      }

      if (const auto duration = frame->GetInt(MapAttribute::Duration))
      {
        frameData.duration = *duration;
      }
      else
      {
        return ParseError::Unknown;  // FIXME
      }
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseCommon(const IMapObject& object,
                               const MapData& data,
                               TilesetData& tileset) -> ParseError
{
  if (const auto tileWidth = object.GetInt(MapAttribute::TileWidth))
  {
    tileset.tile_width = *tileWidth;
  }
  else
  {
    return ParseError::TilesetMissingTileWidth;
  }

  if (const auto tileHeight = object.GetInt(MapAttribute::TileHeight))
  {
    tileset.tile_height = *tileHeight;
  }
  else
  {
    return ParseError::TilesetMissingTileHeight;
  }

  const auto relativeImagePath = object.GetTilesetImageRelativePath();
  if (!relativeImagePath)
  {
    return ParseError::TilesetMissingImagePath;
  }

  const auto directory = data.absolute_path.parent_path();
  const auto absoluteImagePath =
      std::filesystem::weakly_canonical(directory / *relativeImagePath);

  if (std::filesystem::exists(absoluteImagePath))
  {
    tileset.absolute_image_path = absoluteImagePath;
  }
  else
  {
    return ParseError::TilesetImageDoesNotExist;
  }

  if (auto name = object.GetString(MapAttribute::Name))
  {
    tileset.name = std::move(*name);
  }
  else
  {
    return ParseError::TilesetMissingName;
  }

  if (const auto error = ParseTiles(object, tileset); error != ParseError::None)
  {
    return error;
  }

  if (const auto error = ParseProperties(object, tileset.properties);
      error != ParseError::None)
  {
    return error;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseExternal(const IMapObject& object,
                                 const MapData& data,
                                 TilesetData& tileset) -> ParseError
{
  assert(object.Contains(MapAttribute::Source));
  const auto source = object.GetString(MapAttribute::Source).value();

  const auto directory = data.absolute_path.parent_path();
  const auto path = std::filesystem::weakly_canonical(directory / source);

  if (const auto external = object.FromFile(path))
  {
    return ParseCommon(*external->GetExternalTileset(), data, tileset);
  }
  else
  {
    return ParseError::CouldNotReadExternalTileset;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseFirstTileId(const IMapObject& object, tile_id& id)
    -> ParseError
{
  if (const auto gid = object.GetInt(MapAttribute::FirstGid))
  {
    id = tile_id{*gid};
  }
  else
  {
    return ParseError::TilesetMissingFirstGid;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseTileset(const IMapObject& object,
                                const MapData& data,
                                TilesetData& tileset) -> ParseError
{
  if (const auto error = ParseFirstTileId(object, tileset.first_id);
      error != ParseError::None)
  {
    return error;
  }

  if (object.Contains(MapAttribute::Source))
  {
    return ParseExternal(object, data, tileset);
  }
  else
  {
    return ParseCommon(object, data, tileset);
  }

  return ParseError::None;
}

}  // namespace

auto ParseTilesets(const IMapFile& file, MapData& data) -> ParseError
{
  if (!file.ContainsTilesets())
  {
    return ParseError::MapMissingTilesets;
  }

  for (const auto& elem : file.GetTilesets())
  {
    TilesetData tilesetData;
    if (const auto error = ParseTileset(*elem, data, tilesetData);
        error != ParseError::None)
    {
      return error;
    }

    data.tilesets.push_back(std::move(tilesetData));
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
