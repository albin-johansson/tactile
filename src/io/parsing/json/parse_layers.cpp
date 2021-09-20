#include "parse_layers.hpp"

#include <string>   // string
#include <utility>  // move

#include "aliases/ints.hpp"
#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(const JSON& json,
                              LayerData& data,
                              usize index,
                              const std::filesystem::path& dir) -> ParseError;

[[nodiscard]] auto ParseGroupLayer(const JSON& json,
                                   LayerData& target,
                                   const usize index,
                                   const std::filesystem::path& dir) -> ParseError
{
  auto& data = target.data.emplace<GroupLayerData>();

  if (const auto it = json.find("layers"); it != json.end()) {
    usize childIndex = 0;
    for (const auto& [key, layer] : it->items()) {
      auto layerData = std::make_unique<LayerData>();

      if (const auto err = ParseLayer(layer, *layerData, childIndex, dir);
          err != ParseError::None)
      {
        return err;
      }

      data.layers.push_back(std::move(layerData));
      ++childIndex;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const JSON& json,
                              LayerData& data,
                              const usize index,
                              const std::filesystem::path& dir) -> ParseError
{
  data.index = index;

  if (const auto it = json.find("id"); it != json.end()) {
    data.id = LayerID{it->get<LayerID::value_type>()};
  }
  else {
    return ParseError::LayerMissingId;
  }

  if (const auto it = json.find("name"); it != json.end()) {
    it->get_to(data.name);
  }
  else {
    data.name = "Layer";
  }

  if (const auto it = json.find("opacity"); it != json.end()) {
    it->get_to(data.opacity);
  }
  else {
    data.opacity = 1.0f;
  }

  if (const auto it = json.find("visible"); it != json.end()) {
    it->get_to(data.is_visible);
  }
  else {
    data.is_visible = true;
  }

  if (const auto it = json.find("type"); it != json.end()) {
    const auto type = it->get<std::string>();
    if (type == "tilelayer") {
      data.type = LayerType::TileLayer;
      if (const auto err = ParseTileLayer(json, data); err != ParseError::None) {
        return err;
      }
    }
    else if (type == "objectgroup") {
      data.type = LayerType::ObjectLayer;
      if (const auto err = ParseObjectLayer(json, data, dir); err != ParseError::None) {
        return err;
      }
    }
    else if (type == "group") {
      data.type = LayerType::GroupLayer;
      if (const auto err = ParseGroupLayer(json, data, index, dir);
          err != ParseError::None) {
        return err;
      }
    }
    else {
      return ParseError::LayerUnknownType;
    }
  }
  else {
    return ParseError::LayerMissingType;
  }

  if (const auto err = ParseProperties(json, data.properties, dir);
      err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseLayers(const JSON& json,
                 std::vector<LayerData>& layers,
                 const std::filesystem::path& dir) -> ParseError
{
  if (const auto it = json.find("layers"); it != json.end()) {
    usize index = 0;
    for (const auto& [key, layer] : it->items()) {
      auto& layerData = layers.emplace_back();

      if (const auto err = ParseLayer(layer, layerData, index, dir);
          err != ParseError::None) {
        return err;
      }

      ++index;
    }
  }
  else {
    return ParseError::MapMissingLayers;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
