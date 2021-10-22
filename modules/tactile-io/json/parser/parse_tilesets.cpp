#include "parse_tilesets.hpp"

#include "parse_tileset.hpp"

namespace Tactile::IO {

auto ParseTilesets(const JSON& json, Map& map, const std::filesystem::path& dir)
    -> ParseError
{
  const auto it = json.find("tilesets");
  if (it == json.end()) {
    return ParseError::MapMissingTilesets;
  }

  ReserveTilesets(map, it->size());

  for (const auto& [key, value] : it->items()) {
    auto& tileset = AddTileset(map);
    if (const auto err = ParseTileset(value, tileset, dir); err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
