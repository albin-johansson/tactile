#include "parse_fancy_tiles.hpp"

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseFancyTile(const JSON& json,
                                  TileData& data,
                                  const std::filesystem::path& dir) -> ParseError
{
  data.id = TileID{json.at("id").get<TileID::value_type>()};

  if (const auto it = json.find("animation"); it != json.end()) {
    for (const auto& [_, frame] : it->items()) {
      auto& frameData = data.animation.emplace_back();
      frameData.tile = TileID{frame.at("tileid").get<TileID::value_type>()};
      frame.at("duration").get_to(frameData.duration);
    }
  }

  if (const auto err = ParseProperties(json, data.properties, dir);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseFancyTiles(const JSON& json,
                     TilesetData& data,
                     const std::filesystem::path& dir) -> ParseError
{
  if (!json.contains("tiles")) {
    return ParseError::None;
  }

  for (const auto& [key, tileJson] : json.at("tiles").items()) {
    auto& tileData = data.tiles.emplace_back();
    if (const auto err = ParseFancyTile(tileJson, tileData, dir); err != ParseError::None)
    {
      return err;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
