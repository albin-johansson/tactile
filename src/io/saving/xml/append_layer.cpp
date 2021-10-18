#include "append_layer.hpp"

#include <sstream>  // stringstream

#include "append_properties.hpp"
#include <tactile-base/tactile_error.hpp>
#include "core/components/group_layer.hpp"
#include "core/components/layer.hpp"
#include "core/components/layer_tree_node.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tile_layer.hpp"
#include "io/preferences.hpp"

namespace Tactile::IO {
namespace {

void AppendTileLayer(pugi::xml_node mapNode,
                     const entt::registry& registry,
                     const entt::entity layerEntity,
                     const Layer& layer,
                     const std::filesystem::path& dir)
{
  const auto& tileLayer = registry.get<TileLayer>(layerEntity);
  const auto& context = registry.get<PropertyContext>(layerEntity);

  auto node = mapNode.append_child("layer");
  node.append_attribute("id").set_value(layer.id.get());
  node.append_attribute("name").set_value(context.name.c_str());

  const auto nCols = tileLayer.matrix.at(0).size();
  const auto nRows = tileLayer.matrix.size();
  node.append_attribute("width").set_value(nCols);
  node.append_attribute("height").set_value(nRows);

  AppendProperties(registry, layerEntity, node, dir);

  auto data = node.append_child("data");
  data.append_attribute("encoding").set_value("csv");

  const auto count = nRows * nCols;
  const auto readable = Prefs::GetHumanReadableOutput();

  std::stringstream stream;
  usize index = 0;

  for (const auto& row : tileLayer.matrix) {
    for (const auto tile : row) {
      if (readable && index == 0) {
        stream << '\n';
      }

      stream << tile.get();
      if (index < count - 1) {
        stream << ',';
      }

      if (readable && (index + 1) % nCols == 0) {
        stream << '\n';
      }

      ++index;
    }
  }

  const auto tileData = stream.str();
  data.text().set(tileData.c_str());
}

void AppendObject(pugi::xml_node node,
                  const entt::registry& registry,
                  const entt::entity entity,
                  const std::filesystem::path& dir)
{
  const auto& object = registry.get<Object>(entity);
  const auto& context = registry.get<PropertyContext>(entity);

  auto objNode = node.append_child("object");
  objNode.append_attribute("id").set_value(object.id.get());

  if (!context.name.empty()) {
    objNode.append_attribute("name").set_value(context.name.data());
  }

  if (!object.custom_type.empty()) {
    objNode.append_attribute("type").set_value(object.custom_type.c_str());
  }

  objNode.append_attribute("x").set_value(object.x);
  objNode.append_attribute("y").set_value(object.y);

  if (object.width != 0) {
    objNode.append_attribute("width").set_value(object.width);
  }

  if (object.height != 0) {
    objNode.append_attribute("height").set_value(object.height);
  }

  if (!object.visible) {
    objNode.append_attribute("visible").set_value(0);
  }

  if (object.type == ObjectType::Point) {
    objNode.append_child("point");
  }
  else if (object.type == ObjectType::Ellipse) {
    objNode.append_child("ellipse");
  }

  AppendProperties(registry, entity, node, dir);
}

void AppendObjectLayer(pugi::xml_node mapNode,
                       const entt::registry& registry,
                       const entt::entity layerEntity,
                       const Layer& layer,
                       const std::filesystem::path& dir)
{
  const auto& objectLayer = registry.get<ObjectLayer>(layerEntity);
  const auto& context = registry.get<PropertyContext>(layerEntity);

  auto node = mapNode.append_child("objectgroup");
  node.append_attribute("id").set_value(layer.id.get());
  node.append_attribute("name").set_value(context.name.c_str());

  AppendProperties(registry, layerEntity, node, dir);

  for (const auto objectEntity : objectLayer.objects) {
    AppendObject(node, registry, objectEntity, dir);
  }
}

void AppendGroupLayer(pugi::xml_node mapNode,
                      const entt::registry& registry,
                      const entt::entity layerEntity,
                      const Layer& layer,
                      const std::filesystem::path& dir)
{
  const auto& treeNode = registry.get<LayerTreeNode>(layerEntity);
  const auto& context = registry.get<PropertyContext>(layerEntity);

  auto node = mapNode.append_child("group");
  node.append_attribute("id").set_value(layer.id.get());
  node.append_attribute("name").set_value(context.name.c_str());

  for (const auto child : treeNode.children) {
    AppendLayer(node, registry, child, dir);
  }
}

}  // namespace

void AppendLayer(pugi::xml_node mapNode,
                 const entt::registry& registry,
                 const entt::entity layerEntity,
                 const std::filesystem::path& dir)
{
  const auto& layer = registry.get<Layer>(layerEntity);
  switch (layer.type) {
    case LayerType::TileLayer: {
      AppendTileLayer(mapNode, registry, layerEntity, layer, dir);
      break;
    }
    case LayerType::ObjectLayer: {
      AppendObjectLayer(mapNode, registry, layerEntity, layer, dir);
      break;
    }
    case LayerType::GroupLayer: {
      AppendGroupLayer(mapNode, registry, layerEntity, layer, dir);
      break;
    }
    default:
      throw TactileError{"Did not recognize layer type when saving as XML!"};
  }
}

}  // namespace Tactile::IO
