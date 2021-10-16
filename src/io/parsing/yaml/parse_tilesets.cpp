#include "parse_tilesets.hpp"

#include <filesystem>  // exists, weakly_canonical
#include <string>      // string
#include <utility>     // move

#include "parse_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseTileset(const TileID first, const std::filesystem::path& path)
    -> Expected<TilesetData, ParseError>
{
  try {
    const auto node = YAML::LoadFile(path.string());
    if (!node) {
      return tl::make_unexpected(ParseError::CouldNotReadExternalTileset);
    }

    TilesetData data;
    data.first_id = first;

    if (auto version = node["version"]) {
      if (version.as<int>() != 1) {
        return tl::make_unexpected(ParseError::TilesetInvalidVersion);
      }
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingVersion);
    }

    if (auto name = node["name"]) {
      data.name = name.as<std::string>();
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingName);
    }

    if (auto count = node["tile-count"]) {
      // TODO data.tile_count = name.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingTileCount);
    }

    if (auto tileWidth = node["tile-width"]) {
      data.tile_width = tileWidth.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingTileWidth);
    }

    if (auto tileHeight = node["tile-height"]) {
      data.tile_height = tileHeight.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingTileHeight);
    }

    if (auto columns = node["column-count"]) {
      // TODO data.column_count = columns.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingColumnCount);
    }

    if (auto rel = node["image-path"]) {
      const auto dir = path.parent_path();
      auto abs = std::filesystem::weakly_canonical(dir / rel.as<std::string>());
      if (std::filesystem::exists(abs)) {
        data.absolute_image_path = std::move(abs);
      }
      else {
        return tl::make_unexpected(ParseError::TilesetImageDoesNotExist);
      }
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingImagePath);
    }

    if (auto imageWidth = node["image-width"]) {
      // TODO data.image_width = imageWidth.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingImageWidth);
    }

    if (auto imageHeight = node["image-height"]) {
      // TODO data.image_height = imageHeight.as<int>();
    }
    else {
      return tl::make_unexpected(ParseError::TilesetMissingImageHeight);
    }

    if (auto seq = node["tiles"]) {
      // TODO
    }

    if (auto props = ParseProperties(node)) {
      data.properties = std::move(*props);
    }
    else {
      return tl::make_unexpected(props.error());
    }

    return data;
  }
  catch (...) {
    return tl::make_unexpected(ParseError::CouldNotReadExternalTileset);
  }
}

}  // namespace

auto ParseTilesets(const YAML::Node& seq, const std::filesystem::path& dir)
    -> Expected<std::vector<TilesetData>, ParseError>
{
  std::vector<TilesetData> tilesets;
  tilesets.reserve(seq.size());

  for (const auto& tilesetRef : seq) {
    auto first = tilesetRef["first-global-id"];
    auto path = tilesetRef["path"];

    if (!first) {
      return tl::make_unexpected(ParseError::TilesetMissingFirstGid);
    }

    if (!path) {
      return tl::make_unexpected(ParseError::MissingTilesetPath);
    }

    const auto source = path.as<std::string>();
    const auto definition = std::filesystem::weakly_canonical(dir / source);

    if (std::filesystem::exists(definition)) {
      const auto firstId = TileID{first.as<TileID::value_type>()};
      if (auto tileset = ParseTileset(firstId, definition)) {
        tilesets.push_back(std::move(*tileset));
      }
      else {
        return tl::make_unexpected(tileset.error());
      }
    }
    else {
      return tl::make_unexpected(ParseError::ExternalTilesetDoesNotExist);
    }
  }

  return tilesets;
}

}  // namespace Tactile::IO
