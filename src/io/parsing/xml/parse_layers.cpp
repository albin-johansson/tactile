#include "parse_layers.hpp"

#include <iterator>  // make_move_iterator
#include <memory>    // make_unique
#include <utility>   // move

#include "aliases/ints.hpp"
#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"
#include "xml_utils.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(pugi::xml_node node, LayerData& layer) -> ParseError;

[[nodiscard]] auto GetLayerNodes(const pugi::xml_node root)
    -> std::vector<pugi::xml_node>
{
  auto nodes = Collect(root, "layer");

  auto objectLayers = root.children("objectgroup");
  nodes.insert(nodes.end(),
               std::make_move_iterator(objectLayers.begin()),
               std::make_move_iterator(objectLayers.end()));

  auto groups = root.children("group");
  nodes.insert(nodes.end(),
               std::make_move_iterator(groups.begin()),
               std::make_move_iterator(groups.end()));

  return nodes;
}

[[nodiscard]] auto ParseGroupLayer(const pugi::xml_node node, LayerData& layer)
    -> ParseError
{
  auto& data = layer.data.emplace<GroupLayerData>();

  for (const auto layerNode : GetLayerNodes(node))
  {
    auto layerData = std::make_unique<LayerData>();

    if (const auto err = ParseLayer(layerNode, *layerData); err != ParseError::None)
    {
      return err;
    }

    data.layers.push_back(std::move(layerData));
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const pugi::xml_node node, LayerData& layer)
    -> ParseError
{
  if (const auto id = GetInt(node, "id"))
  {
    layer.id = LayerID{*id};
  }
  else
  {
    return ParseError::LayerMissingId;
  }

  layer.name = GetString(node, "name").value_or("Layer");
  layer.opacity = GetFloat(node, "opacity").value_or(1.0f);
  layer.is_visible = GetBool(node, "visible").value_or(true);

  if (std::strcmp(node.name(), "layer") == 0)
  {
    layer.type = LayerType::TileLayer;
    if (const auto err = ParseTileLayer(node, layer); err != ParseError::None)
    {
      return err;
    }
  }
  else if (std::strcmp(node.name(), "objectgroup") == 0)
  {
    layer.type = LayerType::ObjectLayer;
    if (const auto err = ParseObjectLayer(node, layer); err != ParseError::None)
    {
      return err;
    }
  }
  else if (std::strcmp(node.name(), "group") == 0)
  {
    layer.type = LayerType::GroupLayer;
    if (const auto err = ParseGroupLayer(node, layer); err != ParseError::None)
    {
      return err;
    }
  }
  else
  {
    return ParseError::LayerUnknownType;
  }

  if (const auto err = ParseProperties(node, layer.properties);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseLayers(const pugi::xml_node root, std::vector<LayerData>& layers)
    -> ParseError
{
  usize index = 0;
  for (const auto node : GetLayerNodes(root))
  {
    auto& data = layers.emplace_back();
    data.index = index;

    if (const auto err = ParseLayer(node, data); err != ParseError::None)
    {
      return err;
    }

    ++index;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
