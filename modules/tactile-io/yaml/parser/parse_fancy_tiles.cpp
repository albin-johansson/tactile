#include "parse_fancy_tiles.hpp"

#include <yaml-cpp/yaml.h>

#include "parse_components.hpp"
#include "parse_object.hpp"
#include "parse_properties.hpp"

namespace tactile::IO {
namespace {

[[nodiscard]] auto ParseAnimationFrame(const YAML::Node& node, Tile& tile) -> ParseError
{
  auto& frame = AddAnimationFrame(tile);

  if (auto id = node["tile"]) {
    SetTile(frame, id.as<int32>());
  }
  else {
    return ParseError::AnimationFrameNoTile;
  }

  if (auto duration = node["duration"]) {
    SetDuration(frame, duration.as<int32>());
  }
  else {
    return ParseError::AnimationFrameNoDuration;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseFancyTile(const YAML::Node& node,
                                  const Map& map,
                                  Tileset& tileset) -> ParseError
{
  auto& tile = AddTile(tileset);

  if (auto id = node["id"]) {
    SetId(tile, id.as<int32>());
  }
  else {
    return ParseError::TileMissingId;
  }

  if (auto seq = node["animation"]) {
    ReserveAnimationFrames(tile, seq.size());
    for (const auto& frameNode : seq) {
      if (const auto err = ParseAnimationFrame(frameNode, tile);
          err != ParseError::None) {
        return err;
      }
    }
  }

  if (auto seq = node["objects"]) {
    ReserveObjects(tile, seq.size());
    for (const auto& objectNode : seq) {
      if (const auto err = ParseObject(objectNode, map, tile); err != ParseError::None) {
        return err;
      }
    }
  }

  if (const auto err = ParseProperties(node, tile); err != ParseError::None) {
    return err;
  }

  if (const auto err = ParseComponents(map, node, tile); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseFancyTiles(const YAML::Node& node, const Map& map, Tileset& tileset)
    -> ParseError
{
  if (auto seq = node["tiles"]) {
    ReserveTiles(tileset, seq.size());

    for (const auto& tileNode : seq) {
      if (const auto err = ParseFancyTile(tileNode, map, tileset);
          err != ParseError::None) {
        return err;
      }
    }
  }

  return ParseError::None;
}

}  // namespace tactile::IO
