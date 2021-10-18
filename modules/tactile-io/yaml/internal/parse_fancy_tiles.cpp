#include "parse_fancy_tiles.hpp"

#include <utility>  // move

#include <yaml-cpp/yaml.h>

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseAnimationFrame(const YAML::Node& node)
    -> tl::expected<FrameData, ParseError>
{
  FrameData frame;

  if (auto id = node["tile"]) {
    frame.tile = TileID{id.as<TileID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::AnimationFrameNoTile);
  }

  if (auto duration = node["duration"]) {
    frame.duration = duration.as<int>();
  }
  else {
    return tl::make_unexpected(ParseError::AnimationFrameNoDuration);
  }

  return frame;
}

[[nodiscard]] auto ParseFancyTile(const YAML::Node& node)
    -> tl::expected<TileData, ParseError>
{
  TileData data;

  if (auto id = node["id"]) {
    data.id = TileID{id.as<TileID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::TileMissingId);
  }

  if (auto animation = node["animation"]) {
    data.animation.reserve(animation.size());
    for (const auto& frameNode : animation) {
      if (auto frame = ParseAnimationFrame(frameNode)) {
        data.animation.push_back(std::move(*frame));
      }
      else {
        return tl::make_unexpected(frame.error());
      }
    }
  }

  if (auto props = ParseProperties(node)) {
    data.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return data;
}

}  // namespace

auto ParseFancyTiles(const YAML::Node& node)
    -> tl::expected<std::vector<TileData>, ParseError>
{
  std::vector<TileData> tiles;

  if (auto seq = node["tiles"]) {
    tiles.reserve(seq.size());

    for (const auto& tileNode : seq) {
      if (auto tile = ParseFancyTile(tileNode)) {
        tiles.push_back(std::move(*tile));
      }
      else {
        return tl::make_unexpected(tile.error());
      }
    }
  }

  return tiles;
}

}  // namespace Tactile::IO
