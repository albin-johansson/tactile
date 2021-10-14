#include "parse_fancy_tiles.hpp"

#include <utility>  // move

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseFancyTile(const JSON& json) -> Expected<TileData, ParseError>
{
  TileData data;
  data.id = TileID{json.at("id").get<TileID::value_type>()};

  if (const auto it = json.find("animation"); it != json.end()) {
    data.animation.reserve(it->size());
    for (const auto& [_, frame] : it->items()) {
      auto& frameData = data.animation.emplace_back();
      frameData.tile = TileID{frame.at("tileid").get<TileID::value_type>()};
      frame.at("duration").get_to(frameData.duration);
    }
  }

  if (auto props = ParseProperties(json)) {
    data.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return data;
}

}  // namespace

auto ParseFancyTiles(const JSON& json) -> Expected<std::vector<TileData>, ParseError>
{
  std::vector<TileData> tiles;

  if (!json.contains("tiles")) {
    return tiles;
  }

  const auto& array = json.at("tiles");
  tiles.reserve(array.size());

  for (const auto& [key, tileJson] : array.items()) {
    if (auto tile = ParseFancyTile(tileJson)) {
      tiles.push_back(std::move(*tile));
    }
    else {
      return tl::make_unexpected(tile.error());
    }
  }

  return tiles;
}

}  // namespace Tactile::IO
