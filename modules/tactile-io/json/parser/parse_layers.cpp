#include "parse_layers.hpp"

#include <string>  // string

#include <tactile_def.hpp>

#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(const JSON& json, Layer& layer, usize index) -> ParseError;

[[nodiscard]] auto ParseGroupLayer(const JSON& json, Layer& layer) -> ParseError
{
  auto& groupLayer = MarkAsGroupLayer(layer);

  if (const auto it = json.find("layers"); it != json.end()) {
    usize childIndex = 0;
    for (const auto& [key, value] : it->items()) {
      auto& childLayer = AddLayer(groupLayer);

      if (const auto err = ParseLayer(value, childLayer, childIndex);
          err != ParseError::None) {
        return err;
      }

      ++childIndex;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const JSON& json, Layer& layer, const usize index)
    -> ParseError
{
  SetIndex(layer, index);

  if (const auto it = json.find("id"); it != json.end()) {
    SetId(layer, it->get<int32>());
  }
  else {
    return ParseError::LayerMissingId;
  }

  if (const auto it = json.find("name"); it != json.end()) {
    SetName(layer, it->get<std::string>().c_str());
  }
  else {
    SetName(layer, "Layer");
  }

  if (const auto it = json.find("opacity"); it != json.end()) {
    SetOpacity(layer, it->get<float>());
  }
  else {
    SetOpacity(layer, 1.0f);
  }

  if (const auto it = json.find("visible"); it != json.end()) {
    SetVisible(layer, it->get<bool>());
  }
  else {
    SetVisible(layer, true);
  }

  if (const auto it = json.find("type"); it != json.end()) {
    const auto type = it->get<std::string>();
    if (type == "tilelayer") {
      if (const auto err = ParseTileLayer(json, layer); err != ParseError::None) {
        return err;
      }
    }
    else if (type == "objectgroup") {
      if (const auto err = ParseObjectLayer(json, layer); err != ParseError::None) {
        return err;
      }
    }
    else if (type == "group") {
      if (const auto err = ParseGroupLayer(json, layer); err != ParseError::None) {
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

  if (const auto err = ParseProperties(json, layer); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseLayers(const JSON& json, Map& map) -> ParseError
{
  if (const auto it = json.find("layers"); it != json.end()) {
    ReserveLayers(map, it->size());

    usize index = 0;
    for (const auto& [key, value] : it->items()) {
      auto& layer = AddLayer(map);

      if (const auto err = ParseLayer(value, layer, index); err != ParseError::None) {
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
