#include "parse_layers.hpp"

#include <string>  // string

#include <tactile_def.hpp>
#include <yaml-cpp/yaml.h>

#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(const YAML::Node& node,
                              Layer& layer,
                              usize index,
                              usize nRows,
                              usize nCols) -> ParseError;

[[nodiscard]] auto ParseGroupLayer(const YAML::Node& node,
                                   Layer& layer,
                                   const usize nRows,
                                   const usize nCols) -> ParseError
{
  auto& groupLayer = MarkAsGroupLayer(layer);

  if (auto seq = node["layers"]) {
    ReserveLayers(groupLayer, seq.size());

    usize index = 0;
    for (const auto& layerNode : seq) {
      auto& child = AddLayer(groupLayer);

      if (const auto err = ParseLayer(layerNode, child, index, nRows, nCols);
          err != ParseError::None) {
        return err;
      }

      ++index;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const YAML::Node& node,
                              Layer& layer,
                              const usize index,
                              const usize nRows,
                              const usize nCols) -> ParseError
{
  SetIndex(layer, index);

  if (auto id = node["id"]) {
    SetId(layer, id.as<int32>());
  }
  else {
    return ParseError::LayerMissingId;
  }

  if (auto opacity = node["opacity"]) {
    SetOpacity(layer, opacity.as<float>());
  }
  else {
    SetOpacity(layer, 1.0f);
  }

  if (auto visible = node["visible"]) {
    SetVisible(layer, visible.as<bool>());
  }
  else {
    SetVisible(layer, true);
  }

  if (auto name = node["name"]) {
    SetName(layer, name.as<std::string>().c_str());
  }
  else {
    SetName(layer, "Layer");
  }

  if (auto type = node["type"]) {
    const auto value = type.as<std::string>();

    if (value == "tile-layer") {
      if (const auto err = ParseTileLayer(node, layer, nRows, nCols);
          err != ParseError::None) {
        return err;
      }
    }
    else if (value == "object-layer") {
      if (const auto err = ParseObjectLayer(node, layer); err != ParseError::None) {
        return err;
      }
    }
    else if (value == "group-layer") {
      if (const auto err = ParseGroupLayer(node, layer, nRows, nCols);
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

  if (const auto err = ParseProperties(node, layer); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseLayers(const YAML::Node& seq, Map& map) -> ParseError
{
  const auto nRows = GetRowCount(map);
  const auto nCols = GetColumnCount(map);

  ReserveLayers(map, seq.size());

  usize index = 0;
  for (const auto& layerNode : seq) {
    auto& layer = AddLayer(map);
    if (const auto err = ParseLayer(layerNode, layer, index, nRows, nCols);
        err != ParseError::None) {
      return err;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
