#include "save_layers.hpp"

#include <sstream>  // stringstream
#include <variant>  // get

#include <tactile_def.hpp>
#include <yaml-cpp/yaml.h>

#include "emit.hpp"
#include "ir_helpers.hpp"
#include "save_object.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {

void SaveTiles(YAML::Emitter& emitter, const Layer& layer, const bool fold)
{
  emitter << YAML::Key << "data";

  const auto& tileLayer = GetTileLayer(layer);
  const auto nRows = GetRowCount(tileLayer);
  const auto nCols = GetColumnCount(tileLayer);

  std::stringstream stream;
  for (usize row = 0; row < nRows; ++row) {
    for (usize col = 0; col < nCols; ++col) {
      if ((fold && col != 0) || (!fold && (row != 0 || col != 0))) {
        stream << ' ';
      }
      stream << GetTile(tileLayer, row, col);
    }

    if (fold && row < (nRows - 1)) {
      stream << '\n';
    }
  }

  if (fold) {
    emitter << YAML::Literal << stream.str();
  }
  else {
    emitter << YAML::Value << stream.str();
  }
}

void SaveObjects(YAML::Emitter& emitter,
                 const Layer& layer,
                 const std::filesystem::path& dir)
{
  const auto& objectLayer = GetObjectLayer(layer);
  if (GetObjectCount(objectLayer) != 0) {
    emitter << YAML::Key << "objects" << YAML::BeginSeq;

    EachObject(objectLayer,
               [&](const Object& object) { SaveObject(emitter, object, dir); });

    emitter << YAML::EndSeq;
  }
}

void SaveLayer(YAML::Emitter& emitter,
               const Layer& layer,
               const std::filesystem::path& dir,
               const EmitterOptions options)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << GetName(layer);
  emitter << YAML::Key << "id" << YAML::Value << GetId(layer);

  if (const auto opacity = GetOpacity(layer); opacity != 1.0f) {
    emitter << YAML::Key << "opacity" << YAML::Value << opacity;
  }

  if (const auto visible = IsVisible(layer); !visible) {
    emitter << YAML::Key << "visible" << YAML::Value << visible;
  }

  emitter << YAML::Key << "type" << YAML::Value;
  switch (GetType(layer)) {
    case LayerType::TileLayer: {
      emitter << "tile-layer";
      SaveTiles(emitter, layer, options & EmitterOption_FoldTileData);
      break;
    }

    case LayerType::ObjectLayer: {
      emitter << "object-layer";
      SaveObjects(emitter, layer, dir);
      break;
    }

    case LayerType::GroupLayer: {
      emitter << "group-layer";

      const auto& groupLayer = GetGroupLayer(layer);
      if (GetLayerCount(groupLayer) != 0) {
        emitter << YAML::Key << "layers" << YAML::BeginSeq;

        EachLayer(groupLayer,
                  [&](const Layer& child) { SaveLayer(emitter, child, dir, options); });

        emitter << YAML::EndSeq;
      }

      break;
    }
  }

  SaveProperties(emitter, layer, dir);

  emitter << YAML::EndMap;
}

void SaveLayers(YAML::Emitter& emitter,
                const Map& map,
                const std::filesystem::path& dir,
                const EmitterOptions options)
{
  if (GetLayerCount(map) != 0) {
    emitter << YAML::Key << "layers" << YAML::BeginSeq;

    EachLayer(map, [&](const Layer& layer) { SaveLayer(emitter, layer, dir, options); });

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
