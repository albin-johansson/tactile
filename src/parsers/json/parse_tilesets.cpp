#include "parse_tilesets.hpp"

#include <utility>  // move

#include "parse_tileset.hpp"

namespace Tactile::IO {

auto ParseTilesets(const JSON& json, const std::filesystem::path& dir)
    -> Expected<std::vector<TilesetData>, ParseError>
{
  const auto it = json.find("tilesets");
  if (it == json.end()) {
    return tl::make_unexpected(ParseError::MapMissingTilesets);
  }

  std::vector<TilesetData> tilesets;
  tilesets.reserve(it->size());

  for (const auto& [key, value] : it->items()) {
    if (auto tileset = ParseTileset(value, dir)) {
      tilesets.push_back(std::move(*tileset));
    }
    else {
      return tl::make_unexpected(tileset.error());
    }
  }

  return tilesets;
}

}  // namespace Tactile::IO
