#include "save_layers.hpp"

#include <sstream>  // stringstream

#include "common/ints.hpp"
#include "core/components/layer.hpp"
#include "core/components/layer_tree_node.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "core/components/parent.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tile_layer.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {

void SaveTiles(YAML::Emitter& emitter,
               const entt::registry& registry,
               const entt::entity entity)
{
  emitter << YAML::Key << "data";

  const auto& tileLayer = registry.get<TileLayer>(entity);
  const auto nRows = tileLayer.matrix.size();

  std::stringstream stream;
  usize index = 0;

  for (const auto& row : tileLayer.matrix) {
    bool first = true;
    for (const auto tile : row) {
      if (!first) {
        stream << ' ';
      }

      stream << tile;
      first = false;
    }

    if (index < nRows - 1u) {
      stream << '\n';
    }

    ++index;
  }

  emitter << YAML::Literal << stream.str();
}

void SaveObject(YAML::Emitter& emitter,
                const entt::registry& registry,
                const entt::entity entity,
                const std::filesystem::path& dir)
{
  const auto& object = registry.get<Object>(entity);
  const auto& context = registry.get<PropertyContext>(entity);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << object.id.get();

  emitter << YAML::Key << "type" << YAML::Value;
  switch (object.type) {
    case ObjectType::Point:
      emitter << "point";
      break;

    case ObjectType::Rectangle:
      emitter << "rect";
      break;

    case ObjectType::Ellipse:
      emitter << "ellipse";
      break;
  }

  if (!context.name.empty()) {
    emitter << YAML::Key << "name" << YAML::Value << context.name;
  }

  if (!object.custom_type.empty()) {
    emitter << YAML::Key << "tag" << YAML::Value << object.custom_type;
  }

  if (!object.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << object.visible;
  }

  if (object.x != 0.0f) {
    emitter << YAML::Key << "x" << YAML::Value << object.x;
  }

  if (object.y != 0.0f) {
    emitter << YAML::Key << "y" << YAML::Value << object.y;
  }

  if (object.width != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << object.width;
  }

  if (object.height != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << object.height;
  }

  SaveProperties(emitter, registry, entity, dir);

  emitter << YAML::EndMap;
}

void SaveObjects(YAML::Emitter& emitter,
                 const entt::registry& registry,
                 const entt::entity entity,
                 const std::filesystem::path& dir)
{
  const auto& objectLayer = registry.get<ObjectLayer>(entity);
  if (!objectLayer.objects.empty()) {
    emitter << YAML::Key << "objects";
    emitter << YAML::BeginSeq;

    for (const auto objectEntity : objectLayer.objects) {
      SaveObject(emitter, registry, objectEntity, dir);
    }

    emitter << YAML::EndSeq;
  }
}

void SaveLayer(YAML::Emitter& emitter,
               const entt::registry& registry,
               const entt::entity entity,
               const std::filesystem::path& dir)
{
  const auto& node = registry.get<LayerTreeNode>(entity);
  const auto& layer = registry.get<Layer>(entity);
  const auto& context = registry.get<PropertyContext>(entity);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << context.name;
  emitter << YAML::Key << "id" << YAML::Value << layer.id.get();

  if (layer.opacity != 1.0f) {
    emitter << YAML::Key << "opacity" << YAML::Value << layer.opacity;
  }

  if (!layer.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << layer.visible;
  }

  emitter << YAML::Key << "type";
  switch (layer.type) {
    case LayerType::TileLayer: {
      emitter << YAML::Value << "tile-layer";
      SaveTiles(emitter, registry, entity);
      break;
    }

    case LayerType::ObjectLayer: {
      emitter << YAML::Value << "object-layer";
      SaveObjects(emitter, registry, entity, dir);
      break;
    }

    case LayerType::GroupLayer: {
      emitter << YAML::Value << "group-layer";

      if (!node.children.empty()) {
        emitter << YAML::Key << "layers" << YAML::BeginSeq;

        for (const auto child : node.children) {
          SaveLayer(emitter, registry, child, dir);
        }

        emitter << YAML::EndSeq;
      }

      break;
    }
  }

  SaveProperties(emitter, registry, entity, dir);

  emitter << YAML::EndMap;
}

void SaveLayers(YAML::Emitter& emitter,
                const entt::registry& registry,
                const std::filesystem::path& dir)
{
  if (!registry.empty<LayerTreeNode>()) {
    emitter << YAML::Key << "layers";
    emitter << YAML::BeginSeq;

    for (auto&& [entity, node] : registry.view<LayerTreeNode>().each()) {
      const auto& parent = registry.get<Parent>(entity);
      if (parent.entity == entt::null) {
        SaveLayer(emitter, registry, entity, dir);
      }
    }

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
