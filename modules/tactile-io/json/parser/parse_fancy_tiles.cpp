#include "parse_fancy_tiles.hpp"

#include "parse_object_layer.hpp"
#include "parse_properties.hpp"

namespace tactile::IO {
namespace {

[[nodiscard]] auto ParseFancyTile(const JSON& json, Tile& tile) -> ParseError
{
  SetId(tile, json.at("id").get<int32>());

  if (const auto it = json.find("animation"); it != json.end()) {
    ReserveAnimationFrames(tile, it->size());

    for (const auto& [key, value] : it->items()) {
      auto& frame = AddAnimationFrame(tile);
      SetTile(frame, value.at("tileid").get<int32>());
      SetDuration(frame, value.at("duration").get<int32>());
    }
  }

  if (const auto it = json.find("objectgroup"); it != json.end()) {
    if (const auto err = ParseObjectLayer(it.value(), tile); err != ParseError::None) {
      return err;
    }
  }

  if (const auto err = ParseProperties(json, tile); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseFancyTiles(const JSON& json, Tileset& tileset) -> ParseError
{
  if (!json.contains("tiles")) {
    return ParseError::None;
  }

  const auto& array = json.at("tiles");
  ReserveTiles(tileset, array.size());

  for (const auto& [key, value] : array.items()) {
    auto& tile = AddTile(tileset);
    if (const auto err = ParseFancyTile(value, tile); err != ParseError::None) {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace tactile::IO
