#include "save_layers.hpp"

#include <sstream>  // stringstream
#include <variant>  // get

#include <tactile-base/tactile_std.hpp>
#include <yaml-cpp/yaml.h>

#include "save_object.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {

void SaveTiles(YAML::Emitter& emitter, const LayerData& layer)
{
  emitter << YAML::Key << "data";

  const auto& tileLayer = std::get<TileLayerData>(layer.data);
  const auto nRows = tileLayer.tiles.size();

  std::stringstream stream;
  usize index = 0;

  for (const auto& row : tileLayer.tiles) {
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

void SaveObjects(YAML::Emitter& emitter,
                 const LayerData& layer,
                 const std::filesystem::path& dir)
{
  const auto& objectLayer = std::get<ObjectLayerData>(layer.data);
  if (!objectLayer.objects.empty()) {
    emitter << YAML::Key << "objects";
    emitter << YAML::BeginSeq;

    for (const auto& object : objectLayer.objects) {
      SaveObject(emitter, object, dir);
    }

    emitter << YAML::EndSeq;
  }
}

void SaveLayer(YAML::Emitter& emitter,
               const LayerData& layer,
               const std::filesystem::path& dir)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << layer.name;
  emitter << YAML::Key << "id" << YAML::Value << layer.id;

  if (layer.opacity != 1.0f) {
    emitter << YAML::Key << "opacity" << YAML::Value << layer.opacity;
  }

  if (!layer.is_visible) {
    emitter << YAML::Key << "visible" << YAML::Value << layer.is_visible;
  }

  emitter << YAML::Key << "type" << YAML::Value;
  switch (layer.type) {
    case LayerType::TileLayer: {
      emitter << "tile-layer";
      SaveTiles(emitter, layer);
      break;
    }

    case LayerType::ObjectLayer: {
      emitter << "object-layer";
      SaveObjects(emitter, layer, dir);
      break;
    }

    case LayerType::GroupLayer: {
      emitter << "group-layer";

      const auto& groupLayer = std::get<GroupLayerData>(layer.data);
      if (!groupLayer.layers.empty()) {
        emitter << YAML::Key << "layers" << YAML::BeginSeq;

        for (const auto& child : groupLayer.layers) {
          SaveLayer(emitter, *child, dir);
        }

        emitter << YAML::EndSeq;
      }

      break;
    }
  }

  SaveProperties(emitter, layer.properties, dir);

  emitter << YAML::EndMap;
}

void SaveLayers(YAML::Emitter& emitter,
                const std::vector<LayerData>& layers,
                const std::filesystem::path& dir)
{
  if (!layers.empty()) {
    emitter << YAML::Key << "layers" << YAML::BeginSeq;

    for (const auto& layer : layers) {
      SaveLayer(emitter, layer, dir);
    }

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
