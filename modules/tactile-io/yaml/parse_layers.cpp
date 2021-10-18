#include "parse_layers.hpp"

#include <string>   // string
#include <utility>  // move

#include <tactile-base/tactile_std.hpp>

#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(const YAML::Node& node, usize index, int nRows, int nCols)
    -> tl::expected<LayerData, ParseError>;

[[nodiscard]] auto ParseGroupLayer(const YAML::Node& node,
                                   const int nRows,
                                   const int nCols)
    -> tl::expected<GroupLayerData, ParseError>
{
  GroupLayerData data;

  if (auto layers = node["layers"]) {
    data.layers.reserve(layers.size());

    usize index = 0;
    for (const auto& layerNode : layers) {
      if (auto layer = ParseLayer(layerNode, index, nRows, nCols)) {
        data.layers.push_back(std::make_unique<LayerData>(std::move(*layer)));
      }
      else {
        return tl::make_unexpected(layer.error());
      }

      ++index;
    }
  }

  return data;
}

[[nodiscard]] auto ParseLayer(const YAML::Node& node,
                              const usize index,
                              const int nRows,
                              const int nCols) -> tl::expected<LayerData, ParseError>
{
  LayerData layer;
  layer.index = index;

  if (auto id = node["id"]) {
    layer.id = LayerID{id.as<LayerID::value_type>()};
  }
  else {
    return tl::make_unexpected(ParseError::LayerMissingId);
  }

  if (auto opacity = node["opacity"]) {
    layer.opacity = opacity.as<float>();
  }
  else {
    layer.opacity = 1.0f;
  }

  if (auto visible = node["visible"]) {
    layer.is_visible = visible.as<bool>();
  }
  else {
    layer.is_visible = true;
  }

  if (auto name = node["name"]) {
    layer.name = name.as<std::string>();
  }
  else {
    layer.name = "Layer";
  }

  if (auto type = node["type"]) {
    const auto value = type.as<std::string>();
    if (value == "tile-layer") {
      layer.type = LayerType::TileLayer;
      if (auto data = ParseTileLayer(node, nRows, nCols)) {
        layer.data.emplace<TileLayerData>(std::move(*data));
      }
      else {
        return tl::make_unexpected(data.error());
      }
    }
    else if (value == "object-layer") {
      layer.type = LayerType::ObjectLayer;
      if (auto data = ParseObjectLayer(node)) {
        layer.data.emplace<ObjectLayerData>(std::move(*data));
      }
      else {
        return tl::make_unexpected(data.error());
      }
    }
    else if (value == "group-layer") {
      layer.type = LayerType::GroupLayer;
      if (auto data = ParseGroupLayer(node, nRows, nCols)) {
        layer.data.emplace<GroupLayerData>(std::move(*data));
      }
      else {
        return tl::make_unexpected(data.error());
      }
    }
    else {
      return tl::make_unexpected(ParseError::LayerUnknownType);
    }
  }
  else {
    return tl::make_unexpected(ParseError::LayerMissingType);
  }

  if (auto props = ParseProperties(node)) {
    layer.properties = std::move(*props);
  }
  else {
    return tl::make_unexpected(props.error());
  }

  return layer;
}

}  // namespace

auto ParseLayers(const YAML::Node& seq, const int nRows, const int nCols)
    -> tl::expected<std::vector<LayerData>, ParseError>
{
  std::vector<LayerData> layers;
  layers.reserve(seq.size());

  usize index = 0;
  for (const auto& layerNode : seq) {
    if (auto layer = ParseLayer(layerNode, index, nRows, nCols)) {
      layers.push_back(std::move(*layer));
    }
    else {
      return tl::make_unexpected(layer.error());
    }

    ++index;
  }

  return layers;
}

}  // namespace Tactile::IO
