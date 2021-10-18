#include "parse_layers.hpp"

#include <string>   // string
#include <utility>  // move

#include <tactile-base/tactile_std.hpp>

#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(const JSON& json, usize index)
    -> tl::expected<LayerData, ParseError>;

[[nodiscard]] auto ParseGroupLayer(const JSON& json)
    -> tl::expected<GroupLayerData, ParseError>
{
  GroupLayerData data;

  if (const auto it = json.find("layers"); it != json.end()) {
    usize childIndex = 0;
    for (const auto& [key, layer] : it->items()) {
      if (auto child = ParseLayer(layer, childIndex)) {
        data.layers.push_back(std::make_unique<LayerData>(std::move(*child)));
      }
      else {
        return tl::make_unexpected(child.error());
      }

      ++childIndex;
    }
  }

  return data;
}

[[nodiscard]] auto ParseLayer(const JSON& json, const usize index)
    -> tl::expected<LayerData, ParseError>
{
  LayerData layer;
  layer.index = index;

  if (const auto it = json.find("id"); it != json.end()) {
    layer.id = LayerID{it->get<LayerID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::LayerMissingId);
  }

  if (const auto it = json.find("name"); it != json.end()) {
    it->get_to(layer.name);
  }
  else {
    layer.name = "Layer";
  }

  if (const auto it = json.find("opacity"); it != json.end()) {
    it->get_to(layer.opacity);
  }
  else {
    layer.opacity = 1.0f;
  }

  if (const auto it = json.find("visible"); it != json.end()) {
    it->get_to(layer.is_visible);
  }
  else {
    layer.is_visible = true;
  }

  if (const auto it = json.find("type"); it != json.end()) {
    const auto type = it->get<std::string>();
    if (type == "tilelayer") {
      layer.type = LayerType::TileLayer;
      if (auto tileLayer = ParseTileLayer(json)) {
        layer.data.emplace<TileLayerData>(std::move(*tileLayer));
      }
      else {
        return tl::make_unexpected(tileLayer.error());
      }
    }
    else if (type == "objectgroup") {
      layer.type = LayerType::ObjectLayer;
      if (auto objectLayer = ParseObjectLayer(json)) {
        layer.data.emplace<ObjectLayerData>(std::move(*objectLayer));
      }
      else {
        return tl::make_unexpected(objectLayer.error());
      }
    }
    else if (type == "group") {
      layer.type = LayerType::GroupLayer;
      if (auto groupLayer = ParseGroupLayer(json)) {
        layer.data.emplace<GroupLayerData>(std::move(*groupLayer));
      }
      else {
        return tl::make_unexpected(groupLayer.error());
      }
    }
    else {
      return tl::make_unexpected(ParseError::LayerUnknownType);
    }
  }
  else {
    return tl::make_unexpected(ParseError::LayerMissingType);
  }

  if (auto props = ParseProperties(json)) {
    layer.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return layer;
}

}  // namespace

auto ParseLayers(const JSON& json) -> tl::expected<std::vector<LayerData>, ParseError>
{
  std::vector<LayerData> layers;

  if (const auto it = json.find("layers"); it != json.end()) {
    usize index = 0;
    for (const auto& [key, value] : it->items()) {
      if (auto layer = ParseLayer(value, index)) {
        layers.push_back(std::move(*layer));
      }
      else {
        return tl::make_unexpected(layer.error());
      }

      ++index;
    }
  }
  else {
    return tl::make_unexpected(ParseError::MapMissingLayers);
  }

  return layers;
}

}  // namespace Tactile::IO
