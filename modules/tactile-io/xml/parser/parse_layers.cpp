#include "parse_layers.hpp"

#include <cstring>  // strcmp
#include <memory>   // make_unique
#include <utility>  // move

#include <tactile_def.hpp>

#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(pugi::xml_node node, Layer& layer, usize index)
    -> ParseError;

[[nodiscard]] auto GetLayerNodes(const pugi::xml_node root) -> std::vector<pugi::xml_node>
{
  std::vector<pugi::xml_node> nodes;

  for (const auto node : root.children()) {
    if (std::strcmp(node.name(), "layer") == 0 ||
        std::strcmp(node.name(), "objectgroup") == 0 ||
        std::strcmp(node.name(), "group") == 0) {
      nodes.push_back(node);
    }
  }

  return nodes;
}

[[nodiscard]] auto ParseGroupLayer(const pugi::xml_node node, Layer& layer) -> ParseError
{
  auto& groupLayer = MarkAsGroupLayer(layer);

  usize childIndex = 0;
  for (const auto layerNode : GetLayerNodes(node)) {
    auto& childLayer = AddLayer(groupLayer);

    if (const auto err = ParseLayer(layerNode, childLayer, childIndex);
        err != ParseError::None) {
      return err;
    }

    ++childIndex;
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const pugi::xml_node node, Layer& layer, const usize index)
    -> ParseError
{
  SetIndex(layer, index);

  if (const auto id = GetInt(node, "id")) {
    SetId(layer, *id);
  }
  else {
    return ParseError::LayerMissingId;
  }

  SetName(layer, node.attribute("name").as_string("Layer"));
  SetOpacity(layer, GetFloat(node, "opacity").value_or(1.0f));
  SetVisible(layer, GetBool(node, "visible").value_or(true));

  if (std::strcmp(node.name(), "layer") == 0) {
    if (const auto err = ParseTileLayer(node, layer); err != ParseError::None) {
      return err;
    }
  }
  else if (std::strcmp(node.name(), "objectgroup") == 0) {
    if (const auto err = ParseObjectLayer(node, layer); err != ParseError::None) {
      return err;
    }
  }
  else if (std::strcmp(node.name(), "group") == 0) {
    if (const auto err = ParseGroupLayer(node, layer); err != ParseError::None) {
      return err;
    }
  }
  else {
    return ParseError::LayerUnknownType;
  }

  if (const auto err = ParseProperties(node, layer); err != ParseError::None) {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseLayers(const pugi::xml_node root, Map& map) -> ParseError
{
  usize index = 0;
  for (const auto node : GetLayerNodes(root)) {
    auto& layer = AddLayer(map);

    if (const auto err = ParseLayer(node, layer, index); err != ParseError::None) {
      return err;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
