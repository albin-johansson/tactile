#include "parse_fancy_tiles.hpp"

#include <utility>  // move

#include <json.hpp>  // json

#include "parse_object_layer.hpp"
#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseFancyTile(const JSON& json) -> tl::expected<TileData, ParseError>
{
  TileData tile;
  tile.id = TileID{json.at("id").get<TileID::value_type>()};

  if (const auto it = json.find("animation"); it != json.end()) {
    tile.animation.reserve(it->size());
    for (const auto& [_, frame] : it->items()) {
      auto& frameData = tile.animation.emplace_back();
      frameData.tile = TileID{frame.at("tileid").get<TileID::value_type>()};
      frame.at("duration").get_to(frameData.duration);
    }
  }

  if (const auto it = json.find("objectgroup"); it != json.end()) {
    if (auto data = ParseObjectLayer(it.value())) {
      tile.objects = std::move(data->objects);
    }
    else {
      return tl::make_unexpected(data.error());
    }
  }

  if (auto props = ParseProperties(json)) {
    tile.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return tile;
}

}  // namespace

auto ParseFancyTiles(const JSON& json) -> tl::expected<std::vector<TileData>, ParseError>
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
